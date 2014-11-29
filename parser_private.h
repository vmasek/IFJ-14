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

static int nt_program(FILE *input, Tree *globals, Tree *functions);
static int nt_var_section(FILE *input, Tree *vars);
static int nt_var_list(FILE *input, Tree *vars);
static int nt_var_sublist(FILE *input, Tree *vars);
static int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var);
static int t_symbol(FILE *input, enum token_symbol symbol);
static int nt_type(FILE *input, Type *vars);
static int nt_func_section(FILE *input, Tree *functions);
static int nt_func(FILE *input, Tree *functions);
static int t_id(FILE *input, cstring **_id);
static int nt_paramlist(FILE *input, Func_record *func, int count);
static int nt_func_body(FILE *input, Tree *functions);
static int nt_comp_cmd(FILE *input, Tree *functions);
static int t_keyword(FILE *input, enum token_keyword keyword);
static int nt_cmd_list(FILE *input, Tree *functions);
static int nt_cmd_sublist(FILE *input, Tree *functions);
static int nt_cmd(FILE *input, Tree *functions);
static int nt_main(FILE *input, Tree *globals, Tree *functions);
static int nt_arg_list(FILE *input, Tree *functions);
static int nt_else(FILE *input, Tree *functions);

#endif
