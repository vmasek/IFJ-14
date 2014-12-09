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
    I_NEG,              // unary -
    I_SUB,      		// -
    I_MUL,				// *
    I_DIV,      		// /
    I_LESS,     		// <
    I_GREATER,  		// >
    I_LESS_EQUAL,   	// <=
    I_GREATER_EQUAL,    // >=
    I_EQUAL,    		// ==
    I_NOT_EQUAL,    	// !=
    I_AND,              // and
    I_OR,               // or
    I_XOR,              // xor
    I_NOT,              // not
    I_LEN,  			// length
    I_COPY, 			// copy
    I_FIND, 			// find
    I_SORT, 			// sort
    I_PREP,             // push uninited local var
    I_PASS,             // push argument
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
 * Macro that is used to get values from top and top of stack and pops twice,
 * using popping spree.
 * Will print debug message if something is wrong.
 */
#define CALCS_STACK_OPERATIONS()                                              \
if(stack_index(calcs, 0, (int*)&types[0], &values[0])==INTERNAL_ERROR)        \
{                                                                             \
	debug("Invalid read from calcs.\n");                                      \
	return INTERNAL_ERROR;                                                    \
}                                                                             \
if(stack_index(calcs, 1, (int*)&types[1], &values[1])==INTERNAL_ERROR)        \
{                                                                             \
	debug("Invalid read from calcs.\n");                                      \
	return INTERNAL_ERROR;                                                    \
}                                                                             \
if(stack_popping_spree(calcs, 2)==INTERNAL_ERROR)                             \
{                                                                             \
	debug("Popping spree calcs error.\n");                                    \
	return INTERNAL_ERROR;                                                    \
}                                                                             \


/**
 * @brief Macro for operation over calcs stack.
 * @return INTERNAL_ERROR if anything is wrong.
 *
 * Macro that is used to get value from top and top of stack and pops once.
 * Will print debug message if something is wrong.
 */
#define CALCS_STACK_OPERATIONS_RESULT()                                       \
	if (stack_index(calcs, 0, (int *)&types[0], &result) == INTERNAL_ERROR)   \
	{                                                                         \
		debug("Invalid read from calcs.\n");                                  \
		return INTERNAL_ERROR;                                                \
	}                                                                         \
	if (stack_pop(calcs) == INTERNAL_ERROR)                                   \
	{                                                                         \
		debug("Error poping calcs.\n");                                       \
		return INTERNAL_ERROR;                                                \
	}                                                                         \


/**
 * @brief Macro for instructions of comparison.
 * @param instruct instruction name.
 * @param operator used in macro.
 * @return INTERNAL_ERROR if anything is wrong. Else continues.
 *
 * Macro that is used to get two values from top of the calcs, then pops them,
 * compare them using @param operator and push result on calcs stack.
 *
 * Will print debug message if something is wrong.
 */
#define COMPARISON_INSTRUCTION(instruct, operator)                            \
    debug(instruct "\n");                                                     \
	CALCS_STACK_OPERATIONS();                                                 \
	if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))                     \
	{                                                                         \
		debug(instruct " - INT\n");                                           \
		result.data.boolean = values[1].data.integer operator values[0].data.integer;        \
		stack_push(calcs, TYPE_BOOL, &(result));                              \
	}                                                                         \
	else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))              \
	{                                                                         \
		debug(instruct " - DOUBLE\n");                                        \
		result.data.boolean = values[1].data.real operator values[0].data.real;              \
		stack_push(calcs, TYPE_BOOL, &(result));                              \
	}                                                                         \
	else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))              \
	{                                                                         \
		debug(instruct " - BOOL\n");                                          \
		result.data.boolean = values[1].data.boolean operator values[0].data.boolean;        \
		stack_push(calcs, TYPE_BOOL, &(result));                              \
	}                                                                         \
	else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))          \
	{                                                                         \
		debug(instruct " - STRING\n");                                        \
		result.data.boolean = (cstr_cmp(values[1].data.string, values[0].data.string) operator 0) ;\
		stack_push(calcs, TYPE_BOOL, &(result));                              \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		debug("Invalid type passed to instruction\n");                        \
		return INTERNAL_ERROR;                                                \
	}                                                                         \
	break;                                                                    \


/**
 * @brief Macro for instructions of logical operators.
 * @param instruct instruction name.
 * @param operator used in macro.
 * @return INTERNAL_ERROR if anything is wrong. Else continues.
 *
 * Macro that is used to get two values from top of the calcs, then pops them,
 * compare them using @param operator and push result on calcs stack.
 *
 * Will print debug message if something is wrong.
 */
#define LOGICAL_OPERATOR_INSTRUCTION(instruct, operator)                      \
	debug(instruct "\n");                                                     \
	CALCS_STACK_OPERATIONS();                                                 \
	if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))                     \
	{                                                                         \
		debug(instruct " - INT\n");                                           \
		result.data.integer = values[1].data.integer operator values[0].data.integer;        \
		stack_push(calcs, TYPE_INT, &(result));                               \
	}                                                                         \
	else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))              \
	{                                                                         \
		debug(instruct " - BOOL\n");                                          \
		result.data.boolean = values[1].data.boolean operator values[0].data.boolean;        \
		stack_push(calcs, TYPE_BOOL, &(result));                              \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		debug("Invalid type passed to instruction\n");                        \
		return INTERNAL_ERROR;                                                \
	}                                                                         \
	break;                                                                    \

#endif
