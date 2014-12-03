/**
 * @file    parser_expr.c
 * @name    Expression Parser
 * @author  Pavel Tobias (xtobia01)
 * @brief   Implementation of precedence-table-based expression parser
 *****************************************************************************/

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "errors.h"
#include "gc.h"
#include "ial.h"
#include "parser_private.h"
#include "scaner.h"
#include "stack.h"

#define RULE_COUNT 18
#define RULE_MAXLEN 8
#define TERM_COUNT 16
#define TREE_COUNT 3

#define TREE_LOCALS 0
#define TREE_GLOBALS 1
#define TREE_FUNCS 2

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

typedef int Handler(Token **, Stack *, Tree **);

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
static Type get_type(Token *token);
static enum terminal get_terminal(Token *token);
static int hold_token(Token **token, FILE *input);
static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees);

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

static enum terminal get_terminal(Token *token)
{
    if (token == NULL)
        return TERM_END;

    switch (token->type) {
    case TOKEN_SYMBOL:
        switch (token->value.value_symbol) {
        case ADDITION:
            return TERM_ADD;
        case SUBSTRACTION:
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

static int handle_add(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);

    if (stack_top(type_stack, (int *)&op2_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS ||
        stack_top(type_stack, (int *)&op1_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS)
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

    if (stack_push(type_stack, result_type, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_call(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type cur_type;
    Tree_Node *node;
    struct func_record *function;
    unsigned param_count;

    if ((node = tree_find_key_ch(trees[TREE_FUNCS],
                                 tokens[0]->value.value_name)) == NULL)
        return SEMANTIC_ERROR;

    function = node->data;
    param_count = function->param_count;

    while (param_count--) {
        if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
            cur_type != function->params[param_count]->type)
            return INCOMPATIBLE_TYPE;
        stack_pop(type_stack);
    }

    if (stack_top(type_stack, (int *)&cur_type, NULL) != SUCCESS ||
        cur_type != TYPE_OTHER)
        return INCOMPATIBLE_TYPE;

    stack_pop(type_stack);

    if (stack_push(type_stack, function->ret_value.type, NULL)
        != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_comp(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(tokens); //TODO ERASE - OPERATOR TOKEN WILL BE NEEDED
    IGNORE_PARAM(trees);

    if (stack_top(type_stack, (int *)&op2_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS ||
        stack_top(type_stack, (int *)&op1_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS)
        return SYNTAX_ERROR;

    if (op1_type != op2_type)
        return INCOMPATIBLE_TYPE;

    if (stack_push(type_stack, TYPE_BOOL, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_div(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type op1_type;
    Type op2_type;

    IGNORE_PARAM(tokens);
    IGNORE_PARAM(trees);

    if (stack_top(type_stack, (int *)&op2_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS ||
        stack_top(type_stack, (int *)&op1_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS)
        return SYNTAX_ERROR;

    if ((op1_type != TYPE_INT && op1_type != TYPE_REAL) ||
        (op2_type != TYPE_INT && op2_type != TYPE_REAL))
        return INCOMPATIBLE_TYPE;

    if (stack_push(type_stack, TYPE_REAL, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}


static int handle_id(Token **tokens, Stack *type_stack, Tree **trees)
{
    Tree_Node *node;

    if ((trees[TREE_LOCALS] == NULL ||
         (node = tree_find_key_ch(trees[TREE_LOCALS],
                                  tokens[0]->value.value_name)) == NULL) &&
        (node = tree_find_key_ch(trees[TREE_GLOBALS],
                                 tokens[0]->value.value_name)) == NULL)
        return SEMANTIC_ERROR;

    if (stack_push(type_stack, ((struct var_record *)node->data)->type,
                   NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_literal(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type type = get_type(tokens[0]);

    IGNORE_PARAM(trees);

    if (stack_push(type_stack, type, NULL) != SUCCESS)
        return INTERNAL_ERROR;

    return SUCCESS;
}

static int handle_sub_mul(Token **tokens, Stack *type_stack, Tree **trees)
{
    Type op1_type;
    Type op2_type;
    Type result_type;

    IGNORE_PARAM(trees);
    IGNORE_PARAM(tokens); //TODO ERASE - OPERATOR TOKEN WILL BE NEEDED

    if (stack_top(type_stack, (int *)&op2_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS ||
        stack_top(type_stack, (int *)&op1_type, NULL) != SUCCESS ||
        stack_pop(type_stack) != SUCCESS)
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

    if (stack_push(type_stack, result_type, NULL) != SUCCESS)
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

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int error;
    bool finished = false;
    Token *input_token;
    Token *stack_token;
    Stack sym_stack;
    Stack type_stack;
    Tree *trees[TREE_COUNT];

    if (input == NULL || globals == NULL || functions == NULL)
        return INTERNAL_ERROR;

    trees[TREE_LOCALS] = locals;
    trees[TREE_GLOBALS] = globals;
    trees[TREE_FUNCS] = functions;
    stack_init(&sym_stack);
    stack_init(&type_stack);

    if ((error = hold_token(&input_token, input)) != SUCCESS)
        goto fail;

    if (get_terminal(input_token) == TERM_END) {
        error = SYNTAX_ERROR;
        goto fail;
    }

    if ((error = stack_push(&sym_stack, SYM_TERM, NULL)) != SUCCESS)
        goto fail;

    while (!finished) {
        stack_read_first_of_type(&sym_stack, SYM_TERM, (void **)&stack_token);
        switch (PREC_TABLE[get_terminal(stack_token)]
                          [get_terminal(input_token)]) {
        case I:
            if ((error = stack_insert(&sym_stack, SYM_TERM, SYM_RS, NULL))
                != SUCCESS)
                goto fail;
        case P:
            if (get_terminal(stack_token) == TERM_ID &&
                get_terminal(input_token) == TERM_LP &&
                stack_push(&type_stack, TYPE_OTHER, NULL) != SUCCESS)
                goto fail;
            if ((error = stack_push(&sym_stack, SYM_TERM, input_token))
                != SUCCESS)
                goto fail;
            if ((error = hold_token(&input_token, input)) != SUCCESS)
                goto fail;
            break;
        case R:
            if ((error = reduce_rule(&sym_stack, &type_stack, trees)) != SUCCESS)
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

    if (type_stack.count != 1)
        error = INCOMPATIBLE_TYPE;

fail:
    stack_free(&sym_stack);
    stack_free(&type_stack);
    gc_free(__FILE__);

    return error;
}

static int reduce_rule(Stack *sym_stack, Stack *type_stack, Tree **trees)
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
                != get_terminal(token_array[index]))
                break;
        }
        if (index != count)
            continue;
        if (stack_push(sym_stack, RULES[i].reduction, NULL) != SUCCESS)
            return INTERNAL_ERROR;
        if (RULES[i].handler == NULL)
            return SUCCESS;
        return RULES[i].handler(token_array, type_stack, trees);
    }

    return SYNTAX_ERROR;
}
