#ifndef __PARSER_PRIVATE_H__
#define __PARSER_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "errors.h"
#include "ial.h"
#include "scaner.h"

typedef struct var_record {
    cstring *id;
    My_value value;
    bool inited;
} Var_record;

typedef struct func_record {
    cstring *id;
    unsigned param_count;
    Var_record **params;
    Type type;
    Tree *locals;
} Func_record;

#endif
