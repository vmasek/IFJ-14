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


typedef enum
{
    I_WRITE,    		// printf
    I_READLN,   		// scanf
    I_ASSIGN,   		// :=
    I_ADD,      		// +
    I_SUB,      		// -
    I_MULTIPLY, 		// *
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
} Instruction_type;

typedef struct
{
    My_value *op1;
    My_value *op2;
    My_value *result;
    int start; 						//because inbuild copy function needs it
    int count;    					//because inbuild copy function needs it
    Instruction_type instruction;
} T_ITEM;


//TODO: ERROR CODES
int interpret(T_ITEM *testValue);

