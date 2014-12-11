/**
 * @file    stack.h
 * @author  Vojtech Mašek (xmasek15), Pavel Tobias (xtobia01)
 * @brief   Implementation of Stack datatype for IFJ projekt
 ****************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

struct stack_node;

typedef enum {
    VALUE_STACK,
    TOKEN_STACK,
    INSTR_STACK
} Stack_type;

typedef struct {
    Stack_type type;
    struct stack_node *top;
    unsigned int count;
} Stack;

/* BASIC OPERATIONS */
int stack_count(Stack *stack);
int stack_free(Stack *stack);
int stack_init(Stack *stack, Stack_type type);
int stack_pop(Stack *stack);
int stack_push(Stack *stack, int type, void *value);
int stack_top(Stack *stack, int *type, void *value);

/* INDEXING OPERATIONS */
int stack_index(Stack *stack, unsigned index, int *type, void *value);
int stack_update(Stack *stack, unsigned index, int type, void *value);

/* MASS OPERATIONS */
int stack_popping_spree(Stack *stack, unsigned count);

/* TOKEN-CENTRIC OPERATIONS */
int stack_insert(Stack *stack, int searched_type, int type, void *value);
int stack_read_first(Stack *stack, int searched_type, void *value);
int stack_uninsert(Stack *stack, int searched_type, int *type, void *value);

/* PRINTING FUNCTIONS */
int stack_print_types(Stack *stack);

///docasne
void stack_print(Stack*);
void stack_print_node(int, void*);

#endif //__STACK_H__
