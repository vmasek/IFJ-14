#ifndef __PARSER_PRIVATE_H__
#define __PARSER_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "errors.h"
#include "gc.h"
#include "ial.h"
#include "interpreter.h"
#include "scaner.h"
#include "variables.h"

typedef struct var_record {
    Value data;
    Type type;
    unsigned index;
} Var_record;

typedef struct func_record {
    unsigned local_count;
    unsigned param_count;
    Var_record **params;
    Var_record ret_value;
    Tree *locals;
    bool defined;
    Instruction *first_instr;
} Func_record;

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions,
               Variables *global_vars, Instruction **instr_ptr);

#endif
