#ifndef __PARSER_PRIVATE_H__
#define __PARSER_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "errors.h"
#include "ial.h"
#include "scaner.h"

typedef struct var_record {
    cstring *id; //TODO IS THIS NECESSARY?
    My_value value;
    bool inited; //TODO IS THIS NECESSARY IN PARSER?
} Var_record;

typedef struct func_record {
    cstring *id; //TODO IS THIS NECESSARY?
    unsigned param_count;
    Var_record **params;
    Var_record *ret_value;
    Type type; //TODO OBSOLETE
    Tree *locals;
} Func_record;

#endif
