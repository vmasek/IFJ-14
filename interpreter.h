/**
 * @file    interpreter.h
 * @brief   Implementation of interpreter
 * @author  Vojtech Mašek (xmasek15), Tomas Paulus (xpaulu01)
 *******************************************************************/

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "errors.h"
#include "stack.h"
#include "variables.h"
#include "instruction.h"




int interpret(Instruction *item, Variables *globals);

int interpret_loop(Instruction *item, Stack *calcs, Stack *locals, Stack *instructions, Variables *globals);


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
	item = item->next_instruction;                                            \
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
	item = item->next_instruction;                                            \
	break;                                                                    \


#endif
