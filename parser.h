#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include "ial.h"
#include "interpreter.h"

int parse(FILE *input, Instruction *first_instr, Variables *vars);

#endif
