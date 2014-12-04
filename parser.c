#include "parser.h"
#include "parser_private.h"

static int nt_program(FILE *input, Tree *globals, Tree *functions);
static int nt_var_section(FILE *input, Tree *vars);
static int nt_var_list(FILE *input, Tree *vars);
static int nt_var_sublist(FILE *input, Tree *vars);
static int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var);
static int t_symbol(FILE *input, enum token_symbol symbol);
static int nt_type(FILE *input, Type *vars);
static int nt_func_section(FILE *input, Tree *globals, Tree *functions);
static int nt_func(FILE *input, Tree *globals, Tree *functions);
static int t_id(FILE *input, cstring **_id);
static int nt_paramlist(FILE *input, Func_record *func, int count);
static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int t_keyword(FILE *input, enum token_keyword keyword);
static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int nt_main(FILE *input, Tree *globals, Tree *functions);
static int nt_arg_list(FILE *input, Tree *locals, Tree *globals, Tree *functions);
static int nt_else(FILE *input, Tree *locals, Tree *globals, Tree *functions);

int parse(FILE *input)
{
    int ret = SUCCESS;
    Tree globals;
    Tree functions;

    tree_init(&globals);
    tree_init(&functions);
    ret = nt_program(input, &globals, &functions);

    tree_free(&globals);
    tree_free(&functions);

    return ret;
}

static int nt_program(FILE *input, Tree *globals, Tree *functions)
{
    int ret;
    CATCH_VALUE(nt_var_section(input, globals), ret);
    CATCH_VALUE(nt_func_section(input, globals, functions), ret);
    CHECK_VALUE(nt_main(input, globals, functions), ret);

    return SUCCESS;
}

static int nt_var_section(FILE *input, Tree *vars)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);

    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars), ret);
        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

static int nt_var_list(FILE *input, Tree *vars)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, false, NULL), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);

    return SUCCESS;
}

static int nt_var_sublist(FILE *input, Tree *vars)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, true, NULL), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);

    return SUCCESS;
}

static int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var)
{
    int ret;
    Token token;
    Var_record *var = NULL;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_ID) {
        MALLOC_VAR(var, __FILE__);
        tree_insert(vars, cstr_create_str(token.value.value_name), var);
        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, &(var->type)), ret);
        if (_var != NULL)
            *_var = var;
        return SUCCESS;
    } else {
        if (eps) {
            unget_token(&token);
            return RETURNING;
        } else {
            return SYNTAX_ERROR;
        }
    }
}

static int t_symbol(FILE *input, enum token_symbol symbol)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == symbol) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

static int nt_type(FILE *input, Type *type)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    ret = (token.type == TOKEN_KEYWORD &&
          (token.value.value_keyword == KEYWORD_INTEGER ||
           token.value.value_keyword == KEYWORD_REAL ||
           token.value.value_keyword == KEYWORD_STRING ||
           token.value.value_keyword == KEYWORD_BOOLEAN));
    switch (token.value.value_keyword) {
    case KEYWORD_INTEGER:
        *type = TYPE_INT;
        break;
    case KEYWORD_REAL:
        *type = TYPE_REAL;
        break;
    case KEYWORD_STRING:
        *type = TYPE_STRING;
        break;
    case KEYWORD_BOOLEAN:
        *type = TYPE_BOOL;
        break;
    default:
        break;
    }
    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

static int nt_func_section(FILE *input, Tree *globals, Tree *functions)
{
    int ret;
    CHECK_VALUE(nt_func(input, globals, functions), ret);
    CHECK_VALUE(nt_func_section(input, globals, functions), ret);

    return SUCCESS;
}

static int nt_func(FILE *input, Tree *globals, Tree *functions)
{
    int ret;
    Token token;
    Func_record *func = NULL;
    //Var_record *params = NULL;
    cstring *id;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FUNCTION) {
        MALLOC_FUNC(func, __FILE__);

        CHECK_VALUE(t_id(input, &id), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
        CATCH_VALUE(nt_paramlist(input, func, 0), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
        CHECK_VALUE(nt_type(input, &(func->ret_value.type)), ret);
        tree_insert(func->locals, id, &(func->ret_value));
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        CHECK_VALUE(nt_func_body(input, func->locals, globals, functions), ret);

        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

static int t_id(FILE *input, cstring **functions)
{
    bool ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    if ((ret = (token.type == TOKEN_ID))) {
        *functions = cstr_create_str(token.value.value_name);
    }

    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

static int nt_paramlist(FILE *input, Func_record *func, int count)
{
    int ret;
    Var_record *var = NULL;
    CHECK_VALUE(nt_var(input, func->locals, true, &var), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    if ((ret = nt_paramlist(input, func, count + 1)) == RETURNING) {
        func->param_count = count;
        func->params = (Var_record **)gc_malloc(__FILE__, count * sizeof(Var_record *));
        if (func->params == NULL) {
            return INTERNAL_ERROR;
        }
        func->params[count - 1] = var;
        return SUCCESS;
    } else if (ret == SUCCESS) {
        func->params[count - 1] = var;
        return SUCCESS;
    } else {
        return ret;
    }

    return SUCCESS;
}

static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FORWARD) {
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        return SUCCESS;
    } else {
        unget_token(&token);
        CATCH_VALUE(nt_var_section(input, functions), ret);
        CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions), ret);
        return SUCCESS;
    }
}

static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int ret;
    CHECK_VALUE(t_keyword(input, KEYWORD_BEGIN), ret);
    CATCH_VALUE(nt_cmd_list(input, locals, globals, functions), ret);
    CHECK_VALUE(t_keyword(input, KEYWORD_END), ret);

    return SUCCESS;
}

static int t_keyword(FILE *input, enum token_keyword keyword)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == keyword) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int ret;
    CATCH_VALUE(nt_cmd_sublist(input, locals, globals, functions), ret);

    return SUCCESS;
}

static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int ret;
    CHECK_VALUE(nt_cmd(input, locals, globals, functions), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_cmd_sublist(input, locals, globals, functions), ret);

    return SUCCESS;
}

static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions)
{
    int ret;
    Token token;
    CHECK_VALUE(get_token(&token, input), ret);
    cstring *id = NULL;

    if (token.type == TOKEN_KEYWORD) {
        if (token.value.value_keyword == KEYWORD_READLN) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(t_id(input, &id), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WRITE) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(nt_arg_list(input, locals, globals, functions), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_IF) {
            CHECK_VALUE(parse_expr(input, locals, globals, functions,
                                   NULL, NULL), ret);
            CHECK_VALUE(t_symbol(input, KEYWORD_THEN), ret);
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions), ret);
            CATCH_VALUE(nt_else(input, locals, globals, functions), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WHILE) {
            CHECK_VALUE(parse_expr(input,locals, globals, functions,
                                   NULL, NULL), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_DO), ret);
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_REPEAT) {
            CHECK_VALUE(nt_cmd_list(input, locals, globals, functions), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_UNTIL), ret);
            CHECK_VALUE(parse_expr(input, locals, globals, functions,
                                   NULL, NULL), ret);
            return SUCCESS;
        }
    } else if (token.type == TOKEN_ID) {
        //TODO: array
        CHECK_VALUE(t_symbol(input, ASSIGNMENT), ret);
        CHECK_VALUE(parse_expr(input, locals, globals, functions,
                               NULL, NULL), ret);
    }
    unget_token(&token);
    return RETURNING;
}

static int nt_main(FILE *input, Tree *globals, Tree *functions)
{
    int ret;
    CHECK_VALUE(nt_comp_cmd(input, NULL, globals, functions), ret);
    CHECK_VALUE(t_symbol(input, DOT), ret);

    return SUCCESS;
}

static int nt_arg_list(FILE *input, Tree *locals, Tree *globals, Tree *functions) {
    int ret;
    Token token;
    CHECK_VALUE(parse_expr(input, locals, globals, functions,
                           NULL, NULL), ret);
    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == COMMA) {
        CHECK_VALUE(nt_arg_list(input, locals, globals, functions),ret);
    } else {
        unget_token(&token);
    }
    return SUCCESS;
}

static int nt_else(FILE *input, Tree *locals, Tree *globals, Tree *functions) {
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_ELSE) {
        CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions), ret);
    }
    unget_token(&token);
    return RETURNING;
}
