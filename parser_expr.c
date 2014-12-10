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

/* MAXIMUM LENGTH OF A REDUCTION RULE */
#define RULE_MAXLEN 8

/* TREE INDICES */
#define TREE_LOCALS 0
#define TREE_GLOBALS 1
#define TREE_FUNCS 2

/* BUILT-IN FUNCTION IDENTIFIERS */
#define BIF_COPY "copy"
#define BIF_FIND "find"
#define BIF_LENGTH "length"
#define BIF_SORT "sort"

/* MACRO COUNTING ITEMS OF ARRAY */
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/* TYPES */
typedef int Handler(Token *, Stack *, Tree **, Variables *, Instruction **);

enum symbol {
    SYM_SNT,        // scalar non-terminal symbol
    SYM_VNT,        // vectorial non-terminal symbol
    SYM_RS,         // start of rule
    SYM_TERM        // terminal symbol
};

enum terminal {
    TERM_NEG,       // unary -
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
    TERM_AND,       // and
    TERM_OR,        // or
    TERM_XOR,       // xor
    TERM_NOT,       // not
    TERM_COMMA,     // ,
    TERM_LP,        // (
    TERM_RP,        // )
    TERM_LIT,       // literal
    TERM_ID,        // identifier
    TERM_END,       // end of expression
    TERM_COUNT      // count of terminal types - if defined last (^_~)
};

enum action {
    S,  // shift
    P,  // push
    R,  // reduce
    E,  // error
    F   // finish
};

/* DECLARATION OF STATIC FUNCTIONS */
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     Instruction *alt_instr);
static Instruction_type get_instr_type(Token *token);
static Type get_type(Token *token);
static enum terminal get_term(Token *token);
static Value get_value(Token *token);
static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr);

/* DECLARATION OF RULE HANDLERS */
static Handler handle_add;
static Handler handle_boolop;
static Handler handle_call;
static Handler handle_comp;
static Handler handle_div;
static Handler handle_id;
static Handler handle_literal;
static Handler handle_neg;
static Handler handle_not;
static Handler handle_sub_mul;

/* TABLE OF OPERATOR PRECEDENCE */
const enum action PREC_TABLE[TERM_COUNT][TERM_COUNT] = {
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R},
    {S, R, R, S, S, R, R, R, R, R, R, S, R, R, S, R, S, R, S, S, R},
    {S, R, R, S, S, R, R, R, R, R, R, S, R, R, S, R, S, R, S, S, R},
    {S, R, R, R, R, R, R, R, R, R, R, R, R, R, S, R, S, R, S, S, R},
    {S, R, R, R, R, R, R, R, R, R, R, R, R, R, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, S, S, S, S, R, R, R, R, R, R, S, S, S, S, R, S, R, S, S, R},
    {S, R, R, R, R, R, R, R, R, R, R, R, R, R, S, R, S, R, S, S, R},
    {S, R, R, S, S, R, R, R, R, R, R, S, R, R, S, R, S, R, S, S, R},
    {S, R, R, S, S, R, R, R, R, R, R, S, R, R, S, R, S, R, S, S, R},
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R},
    {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, R, S, R, S, S, E},
    {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, P, S, S, E},
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, E, R, E, E, R},
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, E, R, E, E, R},
    {R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, P, R, E, E, R},
    {S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, E, S, E, S, S, F}
};

/* SET OF REDUCTION RULES */
const struct {
    unsigned length;
    struct {
        enum symbol sym_type;
        enum terminal term_type;
    } symbols[RULE_MAXLEN];
    enum symbol reduction;
    Handler *handler;
} RULES[] = {
    {1, {{SYM_TERM, TERM_ID}}, SYM_SNT, handle_id},
    {1, {{SYM_TERM, TERM_LIT}}, SYM_SNT, handle_literal},
    {2, {{SYM_TERM, TERM_NEG}, {SYM_SNT}}, SYM_SNT, handle_neg},
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
    {3, {{SYM_SNT}, {SYM_TERM, TERM_AND}, {SYM_SNT}}, SYM_SNT, handle_boolop},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_OR}, {SYM_SNT}}, SYM_SNT, handle_boolop},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_XOR}, {SYM_SNT}}, SYM_SNT, handle_boolop},
    {2, {{SYM_TERM, TERM_NOT}, {SYM_SNT}}, SYM_SNT, handle_not},
    {3, {{SYM_SNT}, {SYM_TERM, TERM_COMMA}, {SYM_SNT}}, SYM_VNT, NULL},
    {3, {{SYM_VNT, 0}, {SYM_TERM, TERM_COMMA}, {SYM_SNT}}, SYM_VNT, NULL},
    {3, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_TERM, TERM_RP}},
     SYM_SNT, handle_call},
    {4, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_SNT},
         {SYM_TERM, TERM_RP}}, SYM_SNT, handle_call},
    {4, {{SYM_TERM, TERM_ID}, {SYM_TERM, TERM_LP}, {SYM_VNT, 0},
         {SYM_TERM, TERM_RP}}, SYM_SNT, handle_call}
};

/* DEFINITION OF PUBLIC FUNCTIONS */
int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions,
               Variables *global_vars, Instruction **instr_ptr, Type *type,
               bool in_arg)
{
    int error;
    bool finished = false;
    Token input_token;
    Token stack_token;
    enum terminal stack_term;
    enum terminal input_term;
    Stack sym_stack;
    Stack type_stack;
    Tree *trees[] = {
        [TREE_LOCALS] = locals,
        [TREE_GLOBALS] = globals,
        [TREE_FUNCS] = functions
    }; 
    
    if (input == NULL || globals == NULL || functions == NULL ||
        global_vars == NULL || instr_ptr == NULL)
        return INTERNAL_ERROR; 

    stack_init(&sym_stack, TOKEN_STACK);
    stack_init(&type_stack, VALUE_STACK);

    if ((error = get_token(&input_token, input)) != SUCCESS)
        goto fail;

    if (get_term(&input_token) == TERM_END) {
        error = SYNTAX_ERROR;
        debug("Syntax error\n");
        goto fail;
    }

    if ((error = stack_push(&sym_stack, SYM_TERM, NULL)) != SUCCESS)
        goto fail;

    while (!finished) {
        stack_read_first(&sym_stack, SYM_TERM, &stack_token);   
        stack_term = get_term(&stack_token);
        input_term = get_term(&input_token);
        switch (PREC_TABLE[stack_term][input_term]) {
        case S:
            if ((error = stack_insert(&sym_stack, SYM_TERM, SYM_RS, NULL))
                != SUCCESS)
                goto fail;
        case P:
            if (stack_term == TERM_ID && input_term == TERM_LP &&
                stack_push(&type_stack, TYPE_OTHER, NULL) != SUCCESS)
                goto fail;
            if ((error = stack_push(&sym_stack, SYM_TERM, &input_token))
                != SUCCESS)
                goto fail;
            if ((error = get_token(&input_token, input)) != SUCCESS)
                goto fail;
            break;
        case R:
            if ((error = reduce_rule(&sym_stack, &type_stack, trees,
                                     global_vars, instr_ptr)) != SUCCESS)
                goto fail;
            break;
        case E:
            if (!in_arg ||
                (input_term != TERM_COMMA && input_term != TERM_RP)) {
                error = SYNTAX_ERROR;
                debug("Syntax error - prec_table[%u][%u]\n", stack_term,
                      input_term);
                goto fail;
            }
        default:
            unget_token(&input_token);
            finished = true;
            break;
        }
    }

    if (stack_count(&type_stack) != 1) { //TODO IS THIS NECESSARY?
        error = INCOMPATIBLE_TYPE;
        debug("Incompatible type\n");
    }

    if (type != NULL)
        stack_top(&type_stack, (int *)type, NULL);

fail:
    stack_free(&sym_stack);
    stack_free(&type_stack);

    return error;
}

/* DEFINITION OF STATIC FUNCTIONS */
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     Instruction *alt_instr)
{
    if (((*instr_ptr)->next_instruction = gc_malloc(GC_INSTR,
                                                    sizeof(Instruction)))
        == NULL)
        return INTERNAL_ERROR;

    *instr_ptr = (*instr_ptr)->next_instruction;
    **instr_ptr = (Instruction) {
        .instruction = type,
        .index = index,
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
    case TERM_AND:
        return I_AND;
    case TERM_OR:
        return I_OR;
    case TERM_XOR:
        return I_XOR;
    case TERM_NOT:
        return I_NOT;
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
        case NEGATION:
            return TERM_NEG;
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
        case KEYWORD_AND:
            return TERM_AND;
        case KEYWORD_OR:
            return TERM_OR;
        case KEYWORD_XOR:
            return TERM_XOR;
        case KEYWORD_NOT:
            return TERM_NOT;
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
    Value value = {.inited = true};

    if (token == NULL)
        return value;

    switch (get_type(token)) {
    case TYPE_INT:
        value.data.integer = token->value.value_int;
        break;
    case TYPE_REAL:
        value.data.real = token->value.value_float;
        break;
    case TYPE_STRING:
        value.data.string = token->value.value_string;
        break;
    case TYPE_BOOL:
        value.data.boolean = token->value.value_keyword == KEYWORD_TRUE;
        break;
    default:
        break;
    }

    return value;
}

static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    unsigned count = 0;
    unsigned index;
    enum symbol sym_array[RULE_MAXLEN];
    Token token_array[RULE_MAXLEN];

    while (stack_uninsert(sym_stack, SYM_RS, (int *)&sym_array[count],
           &token_array[count]) == SUCCESS)
        count++;

    stack_pop(sym_stack);

    for (unsigned i = 0; i < ARRAY_COUNT(RULES); i++) {
        if (RULES[i].length != count)
            continue;
        for (index = 0; index < count; index++) {
            if (RULES[i].symbols[index].sym_type != sym_array[index])
                break;
            if (sym_array[index] == SYM_TERM &&
                RULES[i].symbols[index].term_type
                != get_term(&token_array[index]))
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

    debug("Syntax error\n");

    return SYNTAX_ERROR;
}

/* DEFINITION OF HANDLER FUNCTIONS */
static int handle_add(Token *tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    switch (op1_type) {
    case TYPE_INT:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        result_type = op2_type;
        break;
    case TYPE_REAL:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        result_type = TYPE_REAL;
        break;
    case TYPE_STRING:
        if (op2_type != TYPE_STRING) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        result_type = TYPE_STRING;
        break;
    default:
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, result_type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_ADD, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_boolop(Token *tokens, Stack *type_stack, Tree **trees,
                         Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    if (op1_type != TYPE_BOOL || op2_type != TYPE_BOOL) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, TYPE_BOOL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, get_instr_type(&tokens[1]), 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_call(Token *tokens, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    Type cur_type;
    Tree_Node *node;
    Instruction_type instr_type;
    struct func_record *function;
    unsigned param_count = 0;
    unsigned local_count = 0;
    struct var_record **locals;
    Instruction *first_instr = NULL;
    Type ret_type;

    IGNORE_PARAM(global_vars);

    stack_print_types(type_stack);

    if (!strcmp(tokens[0].value.value_name, BIF_COPY)) {
        if (stack_index(type_stack, 2, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING ||
            stack_index(type_stack, 1, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_INT ||
            stack_index(type_stack, 0, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_INT) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        instr_type = I_COPY;
        param_count = 3;
        ret_type = TYPE_STRING;
    } else if (!strcmp(tokens[0].value.value_name, BIF_FIND)) {
        if (stack_index(type_stack, 1, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING ||
            stack_index(type_stack, 0, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        instr_type = I_FIND;
        param_count = 2;
        ret_type = TYPE_INT;
    } else if (!strcmp(tokens[0].value.value_name, BIF_LENGTH)) {
        if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        instr_type = I_LEN;
        param_count = 1;
        ret_type = TYPE_INT;
    } else if (!strcmp(tokens[0].value.value_name, BIF_SORT)) {
        if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != TYPE_STRING) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        instr_type = I_SORT;
        param_count = 1;
        ret_type = TYPE_STRING;
    } else { 
        if ((node = tree_find_key_ch(trees[TREE_FUNCS],
                                     tokens[0].value.value_name)) == NULL) {
            debug("Semantic error\n");
            return SEMANTIC_ERROR;
        }
        function = node->data;
        instr_type = I_CALL;
        param_count = function->param_count;
        local_count = function->local_count;
        ret_type = function->ret_value.type;
        first_instr = function->first_instr;
        locals = function->params;
        for (unsigned i = 0; i < param_count; i++) {
            if (stack_index(type_stack, i, (int *)&cur_type, NULL) != SUCCESS ||
                cur_type != function->params[param_count - i]->type) {
                debug("Incompatible type\n");
                return INCOMPATIBLE_TYPE;
            }
        }
    }

    if (stack_index(type_stack, param_count, (int *)&cur_type, NULL)
        != SUCCESS || cur_type != TYPE_OTHER) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, param_count + 1);

    if (stack_push(type_stack, ret_type, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    if (instr_type == I_CALL) {
        while (local_count--) {
            if (gen_instr(instr_ptr, 
                !local_count || local_count > param_count ? I_PREP : I_PASS,
                locals[local_count]->type, NULL) != SUCCESS)
                return INTERNAL_ERROR;
        }
    }

    if (gen_instr(instr_ptr, instr_type, 0, first_instr) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_comp(Token *tokens, Stack *type_stack, Tree **trees,
                       Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    if (op1_type != op2_type) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, TYPE_BOOL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, get_instr_type(&tokens[1]), 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_div(Token *tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    if ((op1_type != TYPE_INT && op1_type != TYPE_REAL) ||
        (op2_type != TYPE_INT && op2_type != TYPE_REAL)) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, TYPE_REAL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_DIV, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_id(Token *tokens, Stack *type_stack, Tree **trees,
                     Variables *global_vars, Instruction **instr_ptr)
{
    Tree_Node *node;
    int index;

    IGNORE_PARAM(global_vars);

    if (trees[TREE_LOCALS] != NULL &&
        (node = tree_find_key_ch(trees[TREE_LOCALS],
                                 tokens[0].value.value_name)) != NULL) {
        index = -((struct var_record *)(node->data))->index - 1;
    } else if ((node = tree_find_key_ch(trees[TREE_GLOBALS],
                                 tokens[0].value.value_name)) != NULL) {
        index = ((struct var_record *)(node->data))->index;
    } else
        return UNDEFINED_IDENTIFIER;

    if (stack_push(type_stack, ((struct var_record *)(node->data))->type, NULL)
        != SUCCESS ||
        gen_instr(instr_ptr, I_PUSH, index, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_literal(Token *tokens, Stack *type_stack, Tree **trees,
                          Variables *global_vars, Instruction **instr_ptr)
{
    Type type = get_type(&tokens[0]);
    unsigned var_index;

    IGNORE_PARAM(trees);

    if (stack_push(type_stack, type, NULL) != SUCCESS ||
        variables_add(global_vars, type, get_value(&tokens[0]), &var_index)
        != SUCCESS ||
        gen_instr(instr_ptr, I_PUSH, var_index, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_neg(Token *tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_top(type_stack, (int *)&type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    if (type != TYPE_INT && type != TYPE_REAL) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_pop(type_stack);

    if (stack_push(type_stack, type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_NEG, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_not(Token *tokens, Stack *type_stack, Tree **trees,
                      Variables *global_vars, Instruction **instr_ptr)
{
    Type type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_top(type_stack, (int *)&type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    if (type != TYPE_BOOL) {
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_pop(type_stack);

    if (stack_push(type_stack, TYPE_BOOL, NULL) != SUCCESS ||
        gen_instr(instr_ptr, I_NOT, 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_sub_mul(Token *tokens, Stack *type_stack, Tree **trees,
                          Variables *global_vars, Instruction **instr_ptr)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(global_vars);

    if (stack_index(type_stack, 1, (int *)&op1_type, NULL) != SUCCESS ||
        stack_index(type_stack, 0, (int *)&op2_type, NULL) != SUCCESS) {
        debug("Syntax error\n");
        return SYNTAX_ERROR;
    }

    switch (op1_type) {
    case TYPE_INT:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        result_type = op2_type;
        break;
    case TYPE_REAL:
        if (op2_type != TYPE_INT && op2_type != TYPE_REAL) {
            debug("Incompatible type\n");
            return INCOMPATIBLE_TYPE;
        }
        result_type = TYPE_REAL;
        break;
    default:
        debug("Incompatible type\n");
        return INCOMPATIBLE_TYPE;
    }

    stack_popping_spree(type_stack, 2);

    if (stack_push(type_stack, result_type, NULL) != SUCCESS ||
        gen_instr(instr_ptr, get_instr_type(&tokens[1]), 0, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}
