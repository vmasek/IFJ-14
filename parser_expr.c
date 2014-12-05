/*
 * @file    parser_expr.c
 * @name    Expression Parser
 * @author  Pavel Tobias (xtobia01)
 * @brief   Implementation of precedence-table-based expression parser
 *****************************************************************************/

#include <stdbool.h>
#include <stdio.h>

#include "parser_private.h"
#include "stack.h"

#define RULE_COUNT 18
#define RULE_MAXLEN 8
#define TERM_COUNT 16
#define TREE_COUNT 3

#define TREE_LOCALS 0
#define TREE_GLOBALS 1
#define TREE_FUNCS 2

#define BIF_COPY "copy"
#define BIF_FIND "find"
#define BIF_LENGTH "length"
#define BIF_SORT "sort"

/* TYPES */
enum symbol {
    SYM_SNT,        // scalar non-terminal symbol
    SYM_VNT,        // vectorial non-terminal symbol
    SYM_RS,         // start of rule
    SYM_TERM        // terminal symbol
};

enum terminal {
    TERM_ADD,       // +
    TERM_SUB,       // -
    TERM_MUL,       // *
    TERM_DIV,       // /
    TERM_EQ,        // =
    TERM_NEQ,       // <>
    TERM_LT,        // <
    TERM_GT,        // >
    TERM_LEQ,       // <=
    TERM_GEQ,       // >=
    TERM_COMMA,     // ,
    TERM_LP,        // (
    TERM_RP,        // )
    TERM_LIT,       // literal
    TERM_ID,        // identifier
    TERM_END        // end of expression
};

enum action {
    I,  // insert
    P,  // push
    R,  // reduce
    E,  // error
    F   // finish
};

typedef int Handler(Token **, Stack *, Tree **, Variables *, Instruction **);

struct rule {
    unsigned length;
    struct {
        enum symbol sym_type;
        enum terminal term_type;
    } symbols[RULE_MAXLEN];
    enum symbol reduction;
    Handler *handler;
};

/* DECLARATION OF STATIC FUNCTIONS */
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     unsigned locals_count, Instruction *alt_instr);
static Instruction_type get_instr_type(Token *token);
static Type get_type(Token *token);
static enum terminal get_term(Token *token);
static Value get_value(Token *token);
static int hold_token(Token **token, FILE *input);
static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr);

/* DECLARATION OF RULE HANDLERS */
static Handler handle_add;
static Handler handle_call;
static Handler handle_comp;
static Handler handle_div;
static Handler handle_id;
static Handler handle_literal;
static Handler handle_sub_mul;

/* CONSTANTS */
const enum action PREC_TABLE[TERM_COUNT][TERM_COUNT] = {
    {R, R, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {R, R, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {R, R, R, R, R, R, R, R, R, R, R, I, R, I, I, R},
    {R, R, R, R, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, R, R, R, R, R, R, R, I, R, I, I, R},
    {I, I, I, I, I, I, I, I, I, I, R, I, R, I, I, E},
    {I, I, I, I, I, I, I, I, I, I, I, I, P, I, I, E},
    {R, R, R, R, R, R, R, R, R, R, R, E, R, E, E, R},
    {R, R, R, R, R, R, R, R, R, R, R, E, R, E, E, R},
    {R, R, R, R, R, R, R, R, R, R, R, P, R, E, E, R},
    {I, I, I, I, I, I, I, I, I, I, E, I, E, I, I, F}
};

const struct rule RULES[RULE_COUNT] = {
    {1, {{SYM_TERM, TERM_ID}}, SYM_SNT, handle_id},
    {1, {{SYM_TERM, TERM_LIT}}, SYM_SNT, handle_literal},
    {3, {{SYM_TERM, TERM_LP}, {SYM_SNT}, {SYM_TERM, TERM_RP}}, SYM_SNT, NULL},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_ADD}, {SYM_SNT}}, SYM_SNT, handle_add},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_SUB}, {SYM_SNT}}, SYM_SNT, handle_sub_mul},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_MUL}, {SYM_SNT}}, SYM_SNT, handle_sub_mul},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_DIV}, {SYM_SNT}}, SYM_SNT, handle_div},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_EQ}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_NEQ}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_LT}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_GT}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_LEQ}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_GEQ}, {SYM_SNT}}, SYM_SNT, handle_comp},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_COMMA}, {SYM_SNT}}, SYM_VNT, NULL},
    {3, {{SYM_VNT, 0}, {SYM_TERM, TERM_COMMA}, {SYM_SNT}}, SYM_VNT, NULL},
    {3, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_TERM, TERM_RP}},
     SYM_SNT, handle_call},
    {4, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_SNT},
         {SYM_TERM, TERM_RP}}, SYM_SNT, handle_call},
    {4, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_VNT, 0},
         {SYM_TERM, TERM_RP}}, SYM_SNT, handle_call}
};

/* DEFINITION OF MISCELLANEOUS FUNCTIONS */
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     unsigned locals_count, Instruction *alt_instr)
{
    if (((*instr_ptr)->next_instruction = gc_malloc(GC_INSTR,
                                                    sizeof(Instruction)))
        == NULL)
        return INTERNAL_ERROR;

    *instr_ptr = (*instr_ptr)->next_instruction;
    **instr_ptr = (Instruction) {
        .instruction = type,
        .index = index,
        .locals_count = locals_count,
        .next_instruction = NULL,
        .alt_instruction = alt_instr
    };

    return SUCCESS;
}

static Instruction_type get_instr_type(Token *token)
{
    if (token == NULL)
        return I_ERR;

    switch (get_term(token)) {
    case TERM_ADD:
        return I_ADD;
    case TERM_SUB:
        return I_SUB;
    case TERM_MUL:
        return I_MUL;
    case TERM_DIV:
        return I_DIV;
    case TERM_EQ:
        return I_EQUAL;
    case TERM_NEQ:
        return I_NOT_EQUAL;
    case TERM_LT:
        return I_LESS;
    case TERM_GT:
        return I_GREATER;
    case TERM_LEQ:
        return I_LESS_EQUAL;
    case TERM_GEQ:
        return I_GREATER_EQUAL;
    default:
        break;
    }

    return I_ERR;
}

static Type get_type(Token *token)
{
    if (token == NULL)
        return TYPE_OTHER;

    switch (token->type) {
    case TOKEN_KEYWORD:
        switch (token->value.value_keyword) {
        case KEYWORD_TRUE:
        case KEYWORD_FALSE:
            return TYPE_BOOL;
        default:
            break;
        }
        break;
    case TOKEN_INT:
        return TYPE_INT;
    case TOKEN_FLOAT:
        return TYPE_REAL;
    case TOKEN_STRING:
        return TYPE_STRING;
    default:
        break;
    }

    return TYPE_OTHER;
}

static enum terminal get_term(Token *token)
{
    if (token == NULL)
        return TERM_END;

    switch (token->type) {
    case TOKEN_SYMBOL:
        switch (token->value.value_symbol) {
        case ADDITION:
            return TERM_ADD;
        case SUBTRACTION:
            return TERM_SUB;
        case MULTIPLICATION:
            return TERM_MUL;
        case DIVISION:
            return TERM_DIV;
        case COMPARISON:
            return TERM_EQ;
        case NOT_EQUAL:
            return TERM_NEQ;
        case LESS_THAN:
            return TERM_LT;
        case GREATER_THAN:
            return TERM_GT;
        case LESS_EQUAL:
            return TERM_LEQ;
        case GREATER_EQUAL:
            return TERM_GEQ;
        case PARENTHESIS_L:
            return TERM_LP;
        case PARENTHESIS_R:
            return TERM_RP;
        case COMMA:
            return TERM_COMMA;
        default:
            break;
        }
        break;
    case TOKEN_KEYWORD:
        switch (token->value.value_keyword) {
        case KEYWORD_TRUE:
        case KEYWORD_FALSE:
            return TERM_LIT;
        default:
            break;
        }
        break;
    case TOKEN_INT:
    case TOKEN_FLOAT:
    case TOKEN_STRING:
        return TERM_LIT;
    case TOKEN_ID:
        return TERM_ID;
    default:
        break;
    }

    return TERM_END;
}

static Value get_value(Token *token)
{
    if (token == NULL)
        return (Value) {.integer = 0};

    switch (get_type(token)) {
    case TYPE_INT:
        return (Value) {.integer = token->value.value_int};
    case TYPE_REAL:
        return (Value) {.real = token->value.value_float};
    case TYPE_STRING:
        return (Value) {.string = token->value.value_string};
    case TYPE_BOOL:
        return (Value) {.boolean = token->value.value_keyword == KEYWORD_TRUE};
    default:
        return (Value) {.integer = 0};
    }

    return (Value) {.integer = 0};
}

static int handle_add(Token **tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS)
        return SYNTAX_ERROR;

    switch (op1_type) {
    case TYPE_INT:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL)
            return INCOMPATIBLE_TYPE;
        result_type = op2_type;
        break;
    case TYPE_REAL:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL)
            return INCOMPATIBLE_TYPE;
        result_type = TYPE_REAL;
        break;
    case TYPE_STRING:
        if (op2_type != TYPE_STRING)
            return INCOMPATIBLE_TYPE;
        result_type = TYPE_STRING;
        break;
    default:
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, result_type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_ADD, 0, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_call(Token **tokens, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    Type cur_type;
    Tree_Node *node;
    Instruction_type instr_type;
    struct func_record *function;
    unsigned param_count = 0;
    unsigned local_count = 0;
    Instruction *first_instr = NULL;
    Type ret_type;

    IGNORE_PARAM(global_vars);

    if (!strcmp(tokens[0]->value.value_name, BIF_COPY)) {
        if (stack_index(type_stack, 2, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING ||
            stack_index(type_stack, 1, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_INT ||
            stack_index(type_stack, 0, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_INT)
            return INCOMPATIBLE_TYPE;
        instr_type = I_COPY;
        param_count = 3;
        ret_type = TYPE_STRING;
    } else if (!strcmp(tokens[0]->value.value_name, BIF_FIND)) {
        if (stack_index(type_stack, 1, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING ||
            stack_index(type_stack, 0, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING)
            return INCOMPATIBLE_TYPE;
        instr_type = I_FIND;
        param_count = 2;
        ret_type = TYPE_INT;
    } else if (!strcmp(tokens[0]->value.value_name, BIF_LENGTH)) {
        if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING)
            return INCOMPATIBLE_TYPE;
        instr_type = I_LEN;
        param_count = 1;
        ret_type = TYPE_INT;
    } else if (!strcmp(tokens[0]->value.value_name, BIF_SORT)) {
        if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING)
            return INCOMPATIBLE_TYPE;
        instr_type = I_SORT;
        param_count = 1;
        ret_type = TYPE_STRING;
    } else { 
        if ((node = tree_find_key_ch(trees[TREE_FUNCS],
                                     tokens[0]->value.value_name)) == NULL)
            return SEMANTIC_ERROR;
        function = node->data;
        instr_type = I_CALL;
        param_count = function->param_count;
        local_count = function->local_count;
        ret_type = function->ret_value.type;
        first_instr = function->first_instr;
        for (unsigned i = 0; i < param_count; i++) {
            if (stack_index(type_stack, i, (int *)&cur_type, NULL) != SUCCESS ||
                cur_type != function->params[param_count - i - 1]->type)
                return INCOMPATIBLE_TYPE;
        }
    }

    if (stack_index(type_stack, param_count, (int *)&cur_type, NULL)
        != SUCCESS || cur_type != TYPE_OTHER)
        return INCOMPATIBLE_TYPE;

    stack_popping_spree(type_stack, param_count + 1);

    if (stack_push(type_stack, ret_type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, instr_type, param_count, local_count, first_instr)
        != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_comp(Token **tokens, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS)
        return SYNTAX_ERROR;

    if (op1_type != op2_type)
        return INCOMPATIBLE_TYPE;

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, TYPE_BOOL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, get_instr_type(tokens[1]), 0, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_div(Token **tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS)
        return SYNTAX_ERROR;

    if ((op1_type != TYPE_INT && op1_type != TYPE_REAL) ||
        (op2_type != TYPE_INT && op2_type != TYPE_REAL))
        return INCOMPATIBLE_TYPE;

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, TYPE_REAL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_DIV, 0, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}


static int handle_id(Token **tokens, Stack *type_stack, Tree **trees,
                     Variables *global_vars, Instruction **instr_ptr)
{
    Tree_Node *node;
    int index;

    IGNORE_PARAM(global_vars);

    if (trees[TREE_LOCALS] != NULL &&
        (node = tree_find_key_ch(trees[TREE_LOCALS],
                                 tokens[0]->value.value_name)) != NULL) {
        index = -((struct var_record *)(node->data))->index - 1;
    } else if ((node = tree_find_key_ch(trees[TREE_GLOBALS],
                                 tokens[0]->value.value_name)) != NULL) {
        index = ((struct var_record *)(node->data))->index;
    } else
        return UNDEFINED_IDENTIFIER;

    if (stack_push(type_stack, ((struct var_record *)(node->data))->type, NULL)
        != SUCCESS ||
        gen_instr(instr_ptr, I_PUSH, index, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_literal(Token **tokens, Stack *type_stack, Tree **trees,
                          Variables *global_vars, Instruction **instr_ptr)
{
    Type type = get_type(tokens[0]);
    unsigned var_index;

    IGNORE_PARAM(trees);

    if (stack_push(type_stack, type, NULL) != SUCCESS ||
        variables_add(global_vars, type, get_value(tokens[0]), &var_index)
        != SUCCESS ||
        gen_instr(instr_ptr, I_PUSH, var_index, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_sub_mul(Token **tokens, Stack *type_stack, Tree **trees,
                          Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS)
        return SYNTAX_ERROR;

    switch (op1_type) {
    case TYPE_INT:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL)
            return INCOMPATIBLE_TYPE;
        result_type = op2_type;
        break;
    case TYPE_REAL:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL)
            return INCOMPATIBLE_TYPE;
        result_type = TYPE_REAL;
        break;
    default:
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, result_type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, get_instr_type(tokens[1]), 0, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int hold_token(Token **token, FILE *input)
{
    if ((*token = gc_malloc(__FILE__, sizeof(Token))) == NULL)
        return INTERNAL_ERROR;

    get_token(*token, input);

    return SUCCESS;
}

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions,
               Variables *global_vars, Instruction **instr_ptr, Type *type)
{
    int error;
    bool finished = false;
    Token *input_token;
    Token *stack_token;
    Stack sym_stack;
    Stack type_stack;
    Tree *trees[TREE_COUNT];

    if (input == NULL || globals == NULL || functions == NULL ||
        global_vars == NULL || instr_ptr == NULL)
        return INTERNAL_ERROR;

    trees[TREE_LOCALS] = locals;
    trees[TREE_GLOBALS] = globals;
    trees[TREE_FUNCS] = functions;
    stack_init(&sym_stack);
    stack_init(&type_stack);

    if ((error = hold_token(&input_token, input)) != SUCCESS)
        goto fail;

    if (get_term(input_token) == TERM_END) {
        error = SYNTAX_ERROR;
        goto fail;
    }

    if ((error = stack_push(&sym_stack, SYM_TERM, NULL)) != SUCCESS)
        goto fail;

    while (!finished) {
        stack_read_first_of_type(&sym_stack, SYM_TERM, (void **)&stack_token);
        switch (PREC_TABLE[get_term(stack_token)][get_term(input_token)]) {
        case I:
            if ((error = stack_insert(&sym_stack, SYM_TERM, SYM_RS, NULL))
                != SUCCESS)
                goto fail;
        case P:
            if (get_term(stack_token) == TERM_ID &&
                get_term(input_token) == TERM_LP &&
                stack_push(&type_stack, TYPE_OTHER, NULL) != SUCCESS)
                goto fail;
            if ((error = stack_push(&sym_stack, SYM_TERM, input_token))
                != SUCCESS)
                goto fail;
            if ((error = hold_token(&input_token, input)) != SUCCESS)
                goto fail;
            break;
        case R:
            if ((error = reduce_rule(&sym_stack, &type_stack, trees,
                                     global_vars, instr_ptr)) != SUCCESS)
                goto fail;
            break;
        case F:
            unget_token(input_token);
            finished = true;
            break;
        default:
            error = SYNTAX_ERROR;
            goto fail;
        }
    }

    if (type_stack.count != 1) //TODO IS THIS NECESSARY?
        error = INCOMPATIBLE_TYPE;

    if (type != NULL)
        stack_top(&type_stack, (int *)type, NULL);

fail:
    stack_free(&sym_stack);
    stack_free(&type_stack);
    gc_free(__FILE__);

    return error;
}

static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    unsigned count = 0;
    unsigned index;
    enum symbol sym_array[RULE_MAXLEN];
    Token *token_array[RULE_MAXLEN];

    while (stack_uninsert(sym_stack, SYM_RS, (int *)&sym_array[count],
           (void **)&token_array[count]) == SUCCESS)
        count++;

    stack_pop(sym_stack);

    for (unsigned i = 0; i < RULE_COUNT; i++) {
        if (RULES[i].length != count)
            continue;
        for (index = 0; index < count; index++) {
            if (RULES[i].symbols[index].sym_type != sym_array[index])
                break;
            if (sym_array[index] == SYM_TERM &&
                RULES[i].symbols[index].term_type
                != get_term(token_array[index]))
                break;
        }
        if (index != count)
            continue;
        if (stack_push(sym_stack, RULES[i].reduction, NULL) != SUCCESS)
            return INTERNAL_ERROR;
        if (RULES[i].handler == NULL)
            return SUCCESS;
        return RULES[i].handler(token_array, type_stack, trees, global_vars,
                                instr_ptr);
    }

    return SYNTAX_ERROR;
}
