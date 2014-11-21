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

    CHECK_VALUE(nt_var_section(input, globals), ret);
    CHECK_VALUE(nt_func_section(input, functions), ret);
    CHECK_VALUE(nt_main(input, globals, functions), ret);

    return SUCCESS;
}

int nt_var_section(FILE *input, Tree vars)
{
    Token token;

    get_token(input, &token);

    if (token.type == TOKEN_KEYWORD ||
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars));
        return SUCCESS;
    }

    unget_token(&token);

    return SUCCESS;
}
