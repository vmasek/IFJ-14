#include "parser.h"
#include "parser_private.h"

#define MODULE "parser"

int parse_expr(FILE *input)
{
    (void)input;
    return SUCCESS;
}

int parse(FILE *input, Tree *globals, Tree *functions)
{
    tree_init(globals);
    tree_init(functions);
    nt_program(input, globals, functions);

    return SUCCESS;
}

int nt_program(FILE *input, Tree *globals, Tree *functions)
{
    int ret;

    CATCH_VALUE(nt_var_section(input, globals), ret);
    CHECK_VALUE(nt_func_section(input, functions), ret);
    CHECK_VALUE(nt_main(input, globals, functions), ret);

    return SUCCESS;
}

int nt_var_section(FILE *input, Tree *vars)
{
    int ret;
    Token token;

    get_token(&token, input);

    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars), ret);
        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

int nt_var_list(FILE *input, Tree *vars)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, false, NULL), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);

    return SUCCESS;
}

int nt_var_sublist(FILE *input, Tree *vars)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, true, NULL), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);

    return SUCCESS;
}

int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var)
{
    int ret;
    Token token;
    Var_record *var = NULL;
    
    get_token(&token, input);
    if (token.type == TOKEN_ID) {
        MALLOC_VAR(var, MODULE);
        tree_insert(vars, cstr_create_str(token.value.value_name), var);
        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, &(var->value.type)), ret);
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

int t_symbol(FILE *input, enum token_symbol symbol)
{
    Token token;
    get_token(&token, input);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == symbol) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

int nt_type(FILE *input, Type *type)
{
    Token token;

    get_token(&token, input);
    bool ret = (token.type == TOKEN_KEYWORD &&
               (token.value.value_keyword == KEYWORD_INTEGER ||
                token.value.value_keyword == KEYWORD_REAL ||
                token.value.value_keyword == KEYWORD_STRING ||
                token.value.value_keyword == KEYWORD_BOOLEAN));
    switch (token.value.value_keyword) {
    case KEYWORD_INTEGER:
        *type = Type_INT;
        break;
    case KEYWORD_REAL:
        *type = Type_DOUBLE;
        break;
    case KEYWORD_STRING:
        *type = Type_STRING;
        break;
    case KEYWORD_BOOLEAN:
        *type = Type_BOOL;
        break;
    default:
        break;
    }
    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

int nt_func_section(FILE *input, Tree *functions) 
{
    int ret;
    CHECK_VALUE(nt_func(input, functions), ret);
    CHECK_VALUE(nt_func_section(input, functions), ret);

    return SUCCESS;
}

int nt_func(FILE *input, Tree *functions)
{
    int ret;
    Token token;
    Func_record *func = NULL;
    //Var_record *params = NULL;
    cstring *id;

    get_token(&token, input);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FUNCTION) {
        MALLOC_FUNC(func, MODULE);

        CHECK_VALUE(t_id(input, &id), ret);
        func->id = id;
        CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
        CATCH_VALUE(nt_paramlist(input, func, 0), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
        CHECK_VALUE(nt_type(input, &(func->type)), ret);
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        CHECK_VALUE(nt_func_body(input, functions), ret);

        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

int t_id(FILE *input, cstring **functions)
{
    bool ret;
    Token token;

    get_token(&token, input);
    if ((ret = (token.type == TOKEN_ID))) {
        *functions = cstr_create_str(token.value.value_name);
    }

    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

int nt_paramlist(FILE *input, Func_record *func, int count)
{
    int ret;
    Var_record *var = NULL;
    CHECK_VALUE(nt_var(input, func->locals, true, &var), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    if ((ret = nt_paramlist(input, func, count + 1)) == RETURNING) {
        func->param_count = count;
        func->params = (Var_record **)gc_malloc(MODULE, count * sizeof(Var_record *));
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

int nt_func_body(FILE *input, Tree *functions)
{
    int ret;
    Token token;

    get_token(&token, input);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FORWARD) {
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        return SUCCESS;
    } else {
        unget_token(&token);
        CATCH_VALUE(nt_var_section(input, functions), ret);
        CHECK_VALUE(nt_comp_cmd(input, functions), ret);
        return SUCCESS;
    }
}

int nt_comp_cmd(FILE *input, Tree *functions)
{
    int ret;
    CHECK_VALUE(t_keyword(input, KEYWORD_BEGIN), ret);
    CATCH_VALUE(nt_cmd_list(input, functions), ret);
    CHECK_VALUE(t_keyword(input, KEYWORD_END), ret);

    return SUCCESS;
}

int t_keyword(FILE *input, enum token_keyword keyword)
{
    Token token;
    get_token(&token, input);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == keyword) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

int nt_cmd_list(FILE *input, Tree *functions)
{
    int ret;
    CATCH_VALUE(nt_cmd_sublist(input, functions), ret);

    return SUCCESS;
}

int nt_cmd_sublist(FILE *input, Tree *functions)
{
    int ret;
    CHECK_VALUE(nt_cmd(input, functions), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_cmd_sublist(input, functions), ret);

    return SUCCESS;
}

int nt_cmd(FILE *input, Tree *functions)
{
    int ret;
    Token token;
    get_token(&token, input);
    cstring *id = NULL;

    if (token.type == TOKEN_KEYWORD) {
        if (token.value.value_keyword == KEYWORD_READLN) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(t_id(input, &id), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WRITE) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(nt_arg_list(input, functions), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_IF) {
            CHECK_VALUE(parse_expr(input), ret);
            CHECK_VALUE(t_symbol(input, KEYWORD_THEN), ret);
            CHECK_VALUE(nt_comp_cmd(input, functions), ret);
            CATCH_VALUE(nt_else(input, functions), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WHILE) {
            CHECK_VALUE(parse_expr(input), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_DO), ret);
            CHECK_VALUE(nt_comp_cmd(input, functions), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_REPEAT) {
            CHECK_VALUE(nt_cmd_list(input, functions), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_UNTIL), ret);
            CHECK_VALUE(parse_expr(input), ret);
            return SUCCESS;
        }
    } else if (token.type == TOKEN_ID) {
        //TODO: array
        CHECK_VALUE(t_symbol(input, ASSIGNMENT), ret);
        CHECK_VALUE(parse_expr(input), ret);
    }
    unget_token(&token);
    return RETURNING;
}

int nt_main(FILE *input, Tree *globals, Tree *functions)
{
    int ret;
    CHECK_VALUE(nt_comp_cmd(input, functions), ret);
    CHECK_VALUE(t_symbol(input, DOT), ret);

    return SUCCESS;
}

int nt_arg_list(FILE *input, Tree *functions) {
    int ret;
    Token token;
    CHECK_VALUE(parse_expr(input), ret);
    get_token(&token, input);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == COMMA) {
        CHECK_VALUE(nt_arg_list(input, functions),ret);
    } else {
        unget_token(&token);
    }
    return SUCCESS;
}

int nt_else(FILE *input, Tree *functions) {
    int ret;
    Token token;

    get_token(&token, input);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_ELSE) {
        CHECK_VALUE(nt_comp_cmd(input, functions), ret);
    }
    unget_token(&token);
    return RETURNING;
}
