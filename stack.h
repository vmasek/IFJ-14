/**
 * @file    stack.h
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
	int type;
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
int    stack_push(Stack*, int, void*);
int    stack_top(Stack*, int*, void**);


/* Advanced operations */
int    stack_read_first_of_type(Stack*, int, void**);
int    stack_insert(Stack*, int, int, void*);							///Inserting before first of type
int    stack_uninsert(Stack*, int, int*, void**);						///Uninserting from before first of type
int    stack_popping_spree(Stack *, unsigned int);

/* Index operations with stack */
int    stack_index(Stack*, unsigned, int*, void**);						///Gets values from index like type and and void*
int    stack_index_value(Stack*, unsigned, int*, Value*);				///Gets values from index like Value struct
int    stack_index_insert_value(Stack*, unsigned int, int, Value*);	///Inserts values from Value to index

/* Printing */
void   stack_print(Stack*);
void   stack_print_node(int, void *);




#endif //__STACK_H__

