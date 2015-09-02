/**
 * @file    parser.h
 * @brief   Parser implementation (header)
 * @author  Albert Uchytil (xuchyt03)
 ****************************************************************************/


#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>

#include "instruction.h"
#include "variables.h"

int parse(FILE *input, Instruction *first_instr, Variables *vars);

#endif
