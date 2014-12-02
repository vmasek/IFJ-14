/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

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


typedef enum
{
    I_WRITE,    		// printf
    I_READLN,   		// scanf
    I_ASSIGN,   		// :=
    I_ADD,      		// +
    I_SUB,      		// -
    I_MUL, 				// *
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
} Instruction_type;


typedef struct Instruction
{
	Instruction_type	instruction;
	int					index;
	struct Instruction	*next_instruction;
	struct Instruction	*alt_instruction;
}Instruction;


int interpret(Instruction *item, Stack *locals, Stack *calcs, Stack *instructions);

