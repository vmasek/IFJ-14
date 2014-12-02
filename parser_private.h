#ifndef __PARSER_PRIVATE_H__
#define __PARSER_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "errors.h"
#include "ial.h"
#include "scaner.h"

typedef struct var_record {
    Value data;
    Type type;
    unsigned index;
} Var_record;

typedef struct func_record {
    unsigned param_count;
    Var_record **params;
    Var_record ret_value;
    Tree *locals;
    bool defined;
    unsigned index;
} Func_record;

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions);

#endif
