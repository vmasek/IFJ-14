/**
 * @file    parser_expr.c
 * @name    Expression Parser
 * @author  Pavel Tobias (xtobia01)
 * @brief   Implementation of precedence-table-based expression parser
 *****************************************************************************/

#ifndef __PARSER_EXPR_H__
#define __PARSER_EXPR_H__

#include <stdio.h>

#include "ial.h"

int parse_expr(FILE *input, Tree *locals, Tree *globals, Tree *functions);

#endif
