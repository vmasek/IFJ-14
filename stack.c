/**
 * @file    stack.c
 * @author  Vojtech Mašek (xmasek15), Pavel Tobias (xtobia01)
 * @brief   Stack datatype
 *
 * Implementation of Stack datatype for IFJ projekt
 ****************************************************************************/

#include <stdlib.h>

#include "instruction.h"
#include "scaner.h"
#include "stack.h"
#include "debug.h"
#include "errors.h"
#include "value.h"

struct stack_node {
    int type;
    union {
        Value value;
        Token token;
        Instruction *instr;
    } value;
    struct stack_node *next;
};

/* STATIC FUNCTIONS */
static void get_value(Stack *stack, struct stack_node *node, void *value)
{
    if (value == NULL)
        return;

    switch (stack->type) {
    case VALUE_STACK:
        *(Value *)value = node->value.value;
        break;
    case TOKEN_STACK:
        *(Token *)value = node->value.token;
        break;
    default:
        *(Instruction **)value = node->value.instr;
        break;
    }
}

static void set_value(Stack *stack, struct stack_node *node, void *value)
{
    if (value == NULL)
        return;

    switch (stack->type) {
    case VALUE_STACK:
        node->value.value = *(Value *)value;
        break;
    case TOKEN_STACK:
        node->value.token = *(Token *)value;
        break;
    default:
        node->value.instr = *(Instruction **)value;
        break;
    }
}

/* BASIC OPERATIONS */
int stack_count(Stack *stack)
{
    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    return stack->count;
}

int stack_free(Stack *stack)
{
    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    return stack_popping_spree(stack, stack->count);
}

int stack_init(Stack *stack, Stack_type type)
{
    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (type != VALUE_STACK && type != TOKEN_STACK && type != INSTR_STACK) {
        debug("Invalid type of stack\n");
        return INTERNAL_ERROR;
    }

    stack->top = NULL;
    stack->type = type;
    stack->count = 0;

    return SUCCESS;
}

int stack_pop(Stack *stack)
{
    struct stack_node *top_node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (!stack->count) {
        debug("Stack is empty\n");
        return INTERNAL_ERROR;
    }

    top_node = stack->top;
    stack->top = top_node->next;
    free(top_node);
    stack->count--;

    return SUCCESS;
}

int stack_push(Stack *stack, int type, void *value)
{
    struct stack_node *new_node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if ((new_node = malloc(sizeof(struct stack_node))) == NULL) {
        debug("Malloc failed\n");
        return INTERNAL_ERROR;
    }

    new_node->next = stack->top;
    stack->top = new_node;
    new_node->type = type;
    stack->count++;
    set_value(stack, new_node, value);

    return SUCCESS;
}

int stack_top(Stack *stack, int *type, void *value)
{
    return stack_index(stack, 0, type, value);
}

/* INDEXING OPERATIONS */
int stack_index(Stack *stack, unsigned index, int *type, void *value)
{
    struct stack_node *node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (index >= stack->count) {
        debug("Stack index %u is out of range\n", index);
        return INTERNAL_ERROR;
    }

    node = stack->top;

    while (index--)
        node = node->next;

    if (type != NULL)
        *type = node->type;

    get_value(stack, node, value);

    return SUCCESS;
}

int stack_update(Stack *stack, unsigned index, int type, void *value)
{
    struct stack_node *node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (index >= stack->count) {
        debug("Stack index %u is out of range\n", index);
        return INTERNAL_ERROR;
    }

    node = stack->top;

    while (index--)
        node = node->next;

    node->type = type;
    set_value(stack, node, value);

    return SUCCESS;
}

/* MASS OPERATIONS */
int stack_popping_spree(Stack *stack, unsigned count)
{
    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (stack->count < count) {
        debug("Popping of %u items, but stack count is only %u\n", count, stack->count);
        return INTERNAL_ERROR;
    }

    while (count--)
        stack_pop(stack);

	return SUCCESS;
}

/* TOKEN-CENTRIC OPERATIONS */
int stack_insert(Stack *stack, int searched_type, int type, void *value)
{
    struct stack_node **found_ptr;
    struct stack_node *new_node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (!stack->count) {
        debug("Stack is empty\n");
        return INTERNAL_ERROR;
    }

    found_ptr = &stack->top;

    for (unsigned i = 0; i < stack->count; i++) {
        if ((*found_ptr)->type != searched_type) {
            found_ptr = &(*found_ptr)->next;
            continue;
        }
        if ((new_node = malloc(sizeof(struct stack_node))) == NULL) {
            debug("Malloc fail");
            return INTERNAL_ERROR;
        }
        new_node->next = *found_ptr;
        new_node->type = type;
        set_value(stack, new_node, value);
        *found_ptr = new_node;
        stack->count++;
        return SUCCESS;
    }

    debug("Node of given type not found\n");

    return INTERNAL_ERROR;
}

int stack_read_first(Stack *stack, int searched_type, void *value)
{
    struct stack_node *node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    node = stack->top;

    for (unsigned i = 0; i < stack->count; i++) {
        if (node->type != searched_type) {
            node = node->next;
            continue;
        }
        get_value(stack, node, value);
        return SUCCESS;
    }

    debug("Node of given type not found\n");

    return INTERNAL_ERROR;
}

int stack_uninsert(Stack *stack, int searched_type, int *type, void *value)
{
    struct stack_node **before_ptr;
    struct stack_node *before;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    if (stack->count < 2) {
        debug("Not enough nodes in stack\n");
        return INTERNAL_ERROR;
    }

    if (stack->top->type == searched_type) {
        debug("Nothing before first found\n");
        return INTERNAL_ERROR;
    }

    before_ptr = &stack->top;

    for (unsigned i = 0; i < stack->count - 1; i++) {
        if ((*before_ptr)->next->type != searched_type) {
            before_ptr = &(*before_ptr)->next;
            continue;
        }
        before = *before_ptr;
        *before_ptr = before->next;
        if (type != NULL)
            *type = before->type;
        get_value(stack, before, value);
        free(before);
        stack->count--;
        return SUCCESS;
    }

    debug("Node of given type not found\n");

    return INTERNAL_ERROR;
}

/* PRINTING FUNCTIONS */
int stack_print_types(Stack *stack)
{
    struct stack_node *node;

    if (stack == NULL) {
        debug("No stack given\n");
        return INTERNAL_ERROR;
    }

    node = stack->top;
    fprintf(stderr, "<stack>\n");

    for (unsigned i = 0; i < stack->count; i++) {
        fprintf(stderr, "  <node type=\"%d\" />\n", node->type);
        node = node->next;
    }

    fprintf(stderr, "</stack>\n");

    return SUCCESS;
}

void stack_print(Stack *stack)
{
	IGNORE_PARAM(stack);
	printf("toto by malo printnut stack ...\n");
}
