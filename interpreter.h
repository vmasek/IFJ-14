/**
 * @file    interpreter.h
 * @brief   Implementation of interpreter
 * @author  Vojtech Mašek (xmasek15), Tomas Paulus (xpaulu01)
 *******************************************************************/

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ial.h"
#include "errors.h"
#include "common.h"
#include "cstring.h"
#include "buildin.h"
#include "stack.h"
#include "variables.h"


typedef enum
{
    I_ERR,				// error !!!
    I_WRITE,    		// printf
    I_READLN,   		// scanf
    I_PUSH,   			// push
    I_ASSIGN,   		// :=
    I_ADD,      		// +
    I_SUB,      		// -
    I_MUL,				// *
    I_DIV,      		// /
    I_LESS,     		// <
    I_GREATER,  		// >
    I_LESS_EQUAL,   	// <=
    I_GREATER_EQUAL,    // >=
    I_EQUAL,    		// ==
    I_NOT_EQUAL,    	// !=
    I_LEN,  			// length
    I_COPY, 			// copy
    I_FIND, 			// find
    I_SORT, 			// sort
	I_JMP,				// jump
	I_CALL,				// call
	I_HALT,				// halt
	I_NOP,				// nop
} Instruction_type;


typedef struct Instruction
{
	Instruction_type	instruction;
	int					index;
	unsigned int		locals_count;
	struct Instruction	*next_instruction;
	struct Instruction	*alt_instruction;
}Instruction;


int interpret(Instruction *item, Stack *calcs, Stack *locals, Stack *instructions, Variables *globals);




/**
 * @brief Macro for operations over calcs stack.
 * @return INTERNAL_ERROR if anything is wrong.
 *
 * Macro that is used to get values from top and top- of stack and pops once.
 * Will print debug message if something is wrong.
 */
#define STACK_OPERATIONS()                                                    \
if(stack_index_value(calcs, 0, (int*)&types[0], &values[0])==INTERNAL_ERROR)  \
{                                                                             \
	debug("Invalid read from calcs\n");                                       \
	return INTERNAL_ERROR;                                                    \
}                                                                             \
if(stack_index_value(calcs, 1, (int*)&types[1], &values[1])==INTERNAL_ERROR)  \
{                                                                             \
	debug("Invalid read from calcs\n");                                       \
	return INTERNAL_ERROR;                                                    \
}                                                                             \
if(stack_popping_spree(calcs, 2)==INTERNAL_ERROR)                             \
{                                                                             \
	debug("Popping spree calcs error\n");                                     \
	return INTERNAL_ERROR;                                                    \
}                                                                             \




#endif
