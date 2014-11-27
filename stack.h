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
#include "errors.h"


typedef struct STACK_NODE {
	Type type;
	void *value;
	struct STACK_NODE *next;
}Stack_Node;


typedef struct STACK {
	Stack_Node *top;
	unsigned int count;
}Stack;



/* Init and Free */
int    stack_init(Stack*);
int    stack_free(Stack*);


/* Basic operations */
int    stack_pop(Stack*);
int    stack_push(Stack*, Type, void*);
int    stack_top(Stack*, Type*, void**);


/* Advanced operations */
int    stack_read_first_of_type(Stack*, Type, void**);
int    stack_insert(Stack*, Type, Type, void*);
int    stack_uninsert(Stack*, Type, Type*, void**);


/* Printing */
void   stack_print(Stack*);
void   stack_print_node(Type, void *);




#endif //__STACK_H__

