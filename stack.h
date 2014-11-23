/**
 * @file    stack.h
 * @name    Stack datatype (header)
 * @author  Vojtech Mašek (xmasek15)
 * @brief   Implementation of Stack datatype for IFJ projekt
 ****************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "cstring.h"



typedef struct STACK_NODE {
	int type;
	void *value;
	struct STACK_NODE *next;
}Stack_Node;


typedef struct STACK {
	Stack_Node *top;
	unsigned int count;
}Stack;

enum Type {
    Type_OTHER = 0,  //  --
    Type_INT,        //  value_int
    Type_DOUBLE,     //  value_double
    Type_CHAR,       //  value_char
    Type_STRING,     //  value_char*
    Type_CSTRING,    //  value_cstring
};




void   stack_init(Stack*);
void   stack_free(Stack*);


void   stack_pop(Stack*);
void   stack_push(Stack*, int, void*);
void   stack_top(Stack*, int*, void*);

void * stack_read_first_of_type(Stack*, int);


void   stack_print(Stack*);
void   stack_print_node(int, void *);


#endif //__STACK_H__

