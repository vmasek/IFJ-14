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
int stack_init(Stack *stack)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}

	stack->top = NULL;
	stack->count = 0;
	return SUCCESS;
}

int stack_push(Stack *stack, int type, void *value)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = malloc(sizeof(Stack_Node)); ///try using calloc and time diff

	if(!tmp)
	{
		return INTERNAL_ERROR;
	}

	tmp->type = type;

	if(!value) {debug("pushing NULL pointer as the value");}

	tmp->value = value;
	tmp->next = stack->top; ///towards

	stack->top = tmp;
	stack->count++;

	return SUCCESS;
}

int stack_top(Stack *stack, int *type, void **value)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack->top;

	*type = tmp->type;
	*value = (tmp->value);
	return SUCCESS;
}

int stack_pop(Stack *stack)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack->top;

	stack->top = tmp->next; ///backwards
	stack->count--;

	free(tmp);
	return SUCCESS;
}

static Stack_Node *stack_find_first_type(Stack *stack, int type)
{
	if(!stack||!stack->top)
	{
		debug("No stack given or stack empty");
		return NULL;
	}

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

int stack_read_first_of_type(Stack *stack, int type, void **value)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack_find_first_type(stack, type);
	if(tmp)
	{
		*value = (tmp->value);
		return SUCCESS;
	}
	else
		return INTERNAL_ERROR;
}

int stack_insert(Stack *stack, int searched_type, int type, void *value)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty");
		return INTERNAL_ERROR;
	}

	Stack_Node *found = stack_find_first_type(stack, searched_type);
	if(found)
	{

		Stack_Node *tmp = malloc(sizeof(Stack_Node)); ///try using calloc and time diff

		if(!tmp)
		{
			debug("Allocation failed");
			return INTERNAL_ERROR;
		}

		tmp->type = type;
		if(!value) {debug("pushing NULL pointer as the value");}
		tmp->value = value;
		tmp->next = found->next;
		found->next = tmp;

		stack->count++;

		return SUCCESS;
	}

	debug("Type not found");
	return INTERNAL_ERROR;
}

int stack_free(Stack *stack)
{
	if(!stack)
	{
		debug("No stack given");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp;

	while((tmp = stack->top))
	{
		stack->top = tmp->next;
		free(tmp);
	}

	stack_init(stack);

	return SUCCESS;
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
	{
		printf("Cannot print stack it is empty\n");
		return;
	}

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
