/**
 * @file    stack.c
 * @name    Stack datatype
 * @author  Vojtech Mašek (xmasek15)
 * @brief   Implementation of Stack datatype for IFJ projekt
 ****************************************************************************/

#include "stack.h"


/**
 * @brief Inicialize stack structure.
 * @param stack poiner to stack structure.
 */
void stack_init(Stack *stack)
{
	if(!stack)
		return;

	stack->top = NULL;
	stack->count = 0;
}

void stack_push(Stack *stack, int type, void *value)
{
	if(!stack)
		return;

	Stack_Node *tmp = malloc(sizeof(Stack_Node)); ///try using calloc and time diff

	tmp->type = type;

	if(!value) {debug("pushing NULL pointer as the value");}

	tmp->value = value;
	tmp->next = stack->top; ///towards

	stack->top = tmp;
	stack->count++;
}

void stack_top(Stack *stack, int *type, void *value)
{
	if(!stack||!stack->top)
		return;

	Stack_Node *tmp = stack->top;

	*type = tmp->type;
	//printf("TOP_DEBUG: value in int representation is ===>   %d\n", *(int*)tmp->value);
	//printf("TOP_DEBUG: value in char representation is ===>   %c\n", *(char*)tmp->value);

	*(int*)value = *(int*)(tmp->value);  ///TODO: this is just wrong, will be rewritten
}

void stack_pop(Stack *stack)
{
	if(!stack||!stack->top)
		return;

	Stack_Node *tmp = stack->top;

	stack->top = tmp->next; ///backwards
	stack->count--;

	free(tmp);
}

static Stack_Node *stack_find_first_type(Stack *stack, int type)
{
	if(!stack||!stack->top)
		return NULL;

	Stack_Node *tmp = stack->top;
	unsigned int count = stack->count;

	while(count)
	{
		if (type == tmp->type)
			return tmp;
		count--;
		tmp=tmp->next;
	}
	debug("type not found in the stack");
	return NULL;
}

void *stack_read_first_of_type(Stack *stack, int type)
{
	if(!stack||!stack->top)
		return NULL;

	Stack_Node *tmp = stack_find_first_type(stack, type);
	if(tmp)
		return tmp->value;
	else
		return NULL;
}

void stack_free(Stack *stack)
{
	if(!stack)
		return;

	Stack_Node *tmp;

	while((tmp = stack->top))
	{
		stack->top = tmp->next;
		free(tmp);
	}

	stack_init(stack);
}






inline void stack_print_node(int type, void *value)
{
	printf("\n\t_________________________________________________________________\n");
	//printf("\t|\ttype:  %d\n", type);
	if (type == Type_INT)									// INT
	{
		if (!value)
		{
			printf("\t|  value:     (int)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value:     (int)[ %d ]\n", *(int*)value);
		}
	}
	else if (type == Type_CHAR)							// CHAR
	{
		if (!value)
		{
			printf("\t|  value:    (char)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value:    (char)[ %c ]\n", *(char*)value);
		}
	}
	else if (type == Type_STRING)							// CHAR*
	{
		if (!value)
		{
			printf("\t|  value:   (char*)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value:   (char*)[ %s ]\n", (char*)value);
		}
	}
	else if (type == Type_CSTRING)							// CSTRING
	{
		if (!value)
		{
			printf("\t|  value: (cstring)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value: (cstring)[ %s ]\n", ((cstring*)value)->str);
		}
	}
	else if (type == Type_DOUBLE)							// DOUBLE
	{
		if (!value)
		{
			printf("\t|  value:  (double)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value:  (double)[ %e ]\n", *(double*)value);
		}
	}
	else if (type == Type_OTHER)							// DOUBLE
	{
		if (!value)
		{
			printf("\t|  value:   (other)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value:   (other)[ pointer to another type value ]\n");
		}
	}
	else
	{
		if (!value)
		{
			printf("\t|  value: (another)[ ivalid or NULL pointer to value ]\n");
		}
		else
		{
			printf("\t|  value: (another)[ valid pointer to another type ('%d') value ]\n", type);
		}
	}
	printf("\t-----------------------------------------------------------------\n");
}



void stack_print(Stack *stack)
{
	if(!stack||!stack->top)
		return;

	Stack_Node *tmp = stack->top;
	unsigned int count = stack->count;

printf("\n===============================Top_of_Stack==============================");

	while(count)
	{
		stack_print_node(tmp->type, tmp->value);

		tmp = tmp->next;
		count--;
	}

printf("\n===============================End_of_Stack==============================\n\n");
}
