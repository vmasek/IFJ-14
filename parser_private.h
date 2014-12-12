/**
 * @file    parser_private.h
 * @brief   Private header for parser
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/


#ifndef __PARSER_PRIVATE_H__
#define __PARSER_PRIVATE_H__

#include <stdbool.h>
#include <stdio.h>

#include "errors.h"
#include "instruction.h"
#include "variables.h"
#include "ial.h"

/* BUILT-IN FUNCTION IDENTIFIERS */
#define BIF_COPY "copy"
#define BIF_FIND "find"
#define BIF_LENGTH "length"
#define BIF_SORT "sort"

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
    Instruction *first_instr;
} Func_record;

/**
 * Macros for memory allocation
 */
#define MALLOC_VAR(val, key) if (((val) = (Var_record *)gc_malloc((key), sizeof(Var_record))) == NULL) return INTERNAL_ERROR
#define MALLOC_FUNC(val, key) if (((val) = (Func_record *)gc_malloc((key), sizeof(Func_record))) == NULL) return INTERNAL_ERROR

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions,
               Variables *global_vars, Instruction **instr_ptr, Type *type,
               bool in_arg);

#endif
