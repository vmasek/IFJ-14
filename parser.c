#include <stdbool.h>

#include "errors.h"
#include "ial.h"
#include "scaner.h"

struct var_record {
    Value value;
    int inited;
};

struct func_record {
    unsigned param_count;
    var_record **params;
    Tree locals;
};

int parse(FILE *input)
{
    Tree globals;
    Tree functions;

    tree_init(globals);
    tree_init(functions);
    nt_program(input, globals, functions);

    return SUCCESS;
}

int nt_program(FILE *input, Tree globals, Tree functions)
{
    int ret;

    CATCH_VALUE(nt_var_section(input, globals), ret);
    CHECK_VALUE(nt_func_section(input, functions), ret);
    CHECK_VALUE(nt_main(input, globals, functions), ret);

    return SUCCESS;
}

int nt_var_section(FILE *input, Tree vars)
{
    Token token;

    get_token(input, &token);

    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars));
        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

int nt_var_list(FILE *input, Tree vars)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, false), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);
}

int nt_var_sublist()
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, true), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_var_sublist(input, vars), ret);
}

int nt_var(FILE *input, Tree vars, bool eps)
{
    int ret;
    Token token;
    
    get_token(input, &token);
    if (token.type == TOKEN_ID) {
        free_token(&token);
        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, vars), ret);
    } else {
        if (eps) {
            unget_token(&token);
            return RETURNING;
        } else {
            free_token(&token);
            return SYNTAX_ERROR;
        }
    }
}

int t_symbol(FILE *input, enum token_symbol symbol)
{
    Token token;
    get_token(input, &token);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == symbol) {
        free_token(&token);
        return SUCCESS;
    }
    free_token(&token);
    return SYNTAX_ERROR;
}

int nt_type(FILE *input, Tree vars)
{
    Token token;

    get_token(input, &token);
    bool ret = (token.type == TOKEN_KEYWORD &&
               (token.value.value_keyword == KEYWORD_INTEGER ||
                token.value.value_keyword == KEYWORD_REAL ||
                token.value.value_keyword == KEYWORD_STRING ||
                token.value.value_keyword == KEYWORD_BOOLEAN));
    free_token(&token);
    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

int nt_func_section(FILE *input, Tree functions) 
{
    int ret;
    CHECK_VALUE(nt_func(input, functions), ret);
    CHECK_VALUE(nt_func_section(input, functions), ret);
}

int nt_func(FILE *input, Tree functions)
{
    int ret;
    Token token;

    get_token(input, &token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FUNCTION) {
        free_token(&token);
        CHECK_VALUE(t_id(input, functions), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
        CATCH_VALUE(nt_paramlist(input, functions), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
        CHECK_VALUE(nt_type(input, functions), ret);
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        CHECK_VALUE(nt_func_body(input, functions), ret);

        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

int t_id(FILE *input, Tree functions)
{
    bool ret;
    Token token;

    get_token(input, &token);
    ret = token.type == TOKEN_ID;
    free_token(&token);

    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

int nt_paramlist(FILE *input, Tree functions)
{
    int ret;
    CHECK_VALUE(nt_var(input, functions, true), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    CHECK_VALUE(nt_paramlist(input, functions), ret);
}

int nt_func_body(FILE *input, Tree functions)
{
    int ret;
    Token token;

    get_token(input, &token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword) {
        free_token(&token);
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        return SUCCESS;
    } else {
        unget_token(&token);
        CATCH_VALUE(nt_var_section(input, functions), ret);
        CHECK_VALUE(nt_comp_cmd(input, functions), ret);
        return SUCCESS;
    }
}

int nt_comp_cmd(FILE *input, Tree functions)
{
    int ret;
    CHECK_VALUE(t_keyword(input, KEYWORD_BEGIN), ret);
    CATCH_VALUE(nt_cmd_list(input, functions), ret);
    CHECK_VALUE(t_keyword(input, KEYWORD_END), ret);
}

int t_keyword(FILE *input, enum token_keyword keyword)
{
    Token token;
    get_token(input, &token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == keyword) {
        free_token(&token);
        return SUCCESS;
    }
    free_token(&token);
    return SYNTAX_ERROR;
}

int nt_cmd_list(FILE *input, Tree functions)
{
    int ret;
    CATCH_VALUE(nt_cmd_sublist(input, functions), ret)
}

int nt_cmd_sublist(FILE *input, Tree functions)
{
    CHECK_VALUE(nt_cmd(input, functions));
    CHECK_VALUE(t_symbol(input, SEMICOLON));
    CHECK_VALUE(nt_cmd_sublist(input, functions));
}

int nt_cmd(FILE *input, Tree functions)
{
    int ret;
    Token token;
    get_token(input, &token);

    if (token.type == KEYWORD) {
        if (token.value.value_keyword == KEYWORD_READLN) {

        } else if (token.value.value_keyword == KEYWORD_WRITELN) {
        
        } else if (token.value.value_keyword == KEYWORD_IF) {

        } else if (token.value.value_keyword == KEYWORD_WHILE) {

        } else if (token.value.value_keyword == KEYWORD_REPEAT) {

        }
    } else if (token.type == TOKEN_ID) {

    }
    //TODO IF
    unget_token(&token);
    return RETURNING;
}

int nt_main(FILE *input, Tree functions)
{
    CHECK_VALUE(nt_comp_cmd(input, functions));
    CHECK_VALUE(t_symbol(input, DOT));
}
