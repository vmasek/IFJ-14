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
		debug("No stack given\n");
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
		debug("No stack given\n");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = malloc(sizeof(Stack_Node)); ///try using calloc and time diff

	if(!tmp)
	{
		return INTERNAL_ERROR;
	}

	tmp->type = type;

	if(!value) {debug("pushing NULL pointer as the value\n");}

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
		debug("No stack given\n");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty\n");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack->top;

	if(type)
		*type = tmp->type;
	if(value)
		*value = tmp->value;
	return SUCCESS;
}


int stack_index(Stack *stack, unsigned index, int *type, void **value)
{
    Stack_Node *node = stack->top;

    if (stack == NULL || stack->top == NULL)
        return INTERNAL_ERROR;

	while (index--) {
		if ((node = node->next) == NULL)
			return INTERNAL_ERROR;
	}

    if (type != NULL)
        *type = node->type;

    if (value != NULL)
        *value = node->value;

    return SUCCESS;
}


int stack_pop(Stack *stack)
{
	if(!stack)
	{
		debug("No stack given\n");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty\n");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack->top;

	stack->top = tmp->next; ///backwards
	stack->count--;

	free(tmp);
	return SUCCESS;
}


int stack_popping_spree(Stack *stack, unsigned int count)
{
	if(stack->count < count)
	{
		debug("Popping of %u items, but stack count is only %u", count, stack->count);
		return INTERNAL_ERROR;
	}

	while(count--)
		stack_pop(stack);

	return SUCCESS;
}


static Stack_Node *stack_find_first_type(Stack *stack, int type)
{
	if(!stack||!stack->top)
	{
		debug("No stack given or stack empty\n");
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
	debug("type not found in the stack\n");
	return NULL;
}


int stack_read_first_of_type(Stack *stack, int type, void **value)
{
	if(!stack)
	{
		debug("No stack given\n");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty\n");
		return INTERNAL_ERROR;
	}

	Stack_Node *tmp = stack_find_first_type(stack, type);
	if(tmp)
	{
		if(value)
			*value = (tmp->value);
		return SUCCESS;
	}
	else
		return INTERNAL_ERROR;
}


int stack_insert(Stack *stack, int searched_type, int type, void *value)
{
    Stack_Node **found_ptr = &stack->top;
    Stack_Node *new_node;

    if (stack == NULL || *found_ptr == NULL)
        return INTERNAL_ERROR;

    for (unsigned i = 0; i < stack->count; i++) {
        if ((*found_ptr)->type != searched_type) {
            found_ptr = &(*found_ptr)->next;
            continue;
        }
        if ((new_node = malloc(sizeof(Stack_Node))) == NULL)
            return INTERNAL_ERROR;
        *new_node = (Stack_Node) {type, value, *found_ptr};
        *found_ptr = new_node;
        stack->count++;
        return SUCCESS;
    }

    return INTERNAL_ERROR;
}


int stack_uninsert(Stack *stack, int searched_type, int *type, void **value)
{
	if(!stack)
	{
		debug("No stack given\n");
		return INTERNAL_ERROR;
	}
	if(!stack->top)
	{
		debug("Stack is empty\n");
		return INTERNAL_ERROR;
	}

	Stack_Node *found = NULL ,
						*before_before_found = stack->top,
						*before_found = stack->top,
						*tmp=stack->top;
	unsigned int count = stack->count;

	//printf("UNINSERT: ____________WHILE_____________\n");
		//printf("UNINSERT: count : %u\n", count);
		//printf("UNINSERT: b_b_f : %p\n", (void*)before_before_found);
		//printf("UNINSERT:   b_f : %p\n", (void*)before_found);
		//printf("UNINSERT:     f : %p\n", (void*)found);
		//printf("UNINSERT:   tmp : %p\n", (void*)tmp);
		//printf("-----------------------------------------\n");
	while(count)
	{
		if (searched_type == tmp->type)
		{
			//printf("UNINSERT: ____________IF(SEARCHED_type==FOUND_type)____\n");
			debug("searched type was found.\n");
			found = tmp;
			//printf("UNINSERT:     f was ok\n");
			break;
		}

		count--;
		before_before_found=before_found;
		//printf("UNINSERT: b_b_f was ok\n");
		before_found=tmp;
		//printf("UNINSERT:   b_f was ok\n");
		tmp=tmp->next;
		//printf("UNINSERT:   tmp was ok\n");
		//printf("UNINSERT: count : %u\n", count);
		//printf("UNINSERT: b_b_f : %p\n", (void*)before_before_found);
		//printf("UNINSERT:   b_f : %p\n", (void*)before_found);
		//printf("UNINSERT:     f : %p\n", (void*)found);
		//printf("UNINSERT:   tmp : %p\n", (void*)tmp);
		//printf("-----------------------------------------\n");
	}

	if(found)
	{
		//printf("UNINSERT: ____________IF(FOUND)____\n");
		if(found==before_found)
		{
			//printf("UNINSERT: ____________IF(FOUND==B_FOUND)____\n");
			debug("found is on top of the stack. Nothing to uninsert.\n");
			return INTERNAL_ERROR;
		}

		if(type)
			*type = before_found->type;

		if(value)
			*value = before_found->value;

		if(before_before_found==before_found)
		{
			debug("found is one node under top of the stack.\n");
			stack->top = found;
		}
		else
		{
			before_before_found->next = found;
		}
		stack->count--;
		free(before_found);
		return SUCCESS;

	}
	//printf("UNINSERT: ____________NOT_FOUND____\n");
	debug("int not found\n");
	return INTERNAL_ERROR;
}


int stack_free(Stack *stack)
{
	if(!stack)
	{
		debug("No stack given\n");
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
			printf("\t|  value:   (other)[ pointer to other type value { %p } ]\n", value);
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
			printf("\t|  value: (another)[ valid pointer to another type ('%d') value { %p } ]\n", type, value);
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

printf("\n===============================Top_of_Stack==============================\n");

	while(count)
	{
#ifdef DEBUG
		printf("Pointer is: [ %p ]\t(long)[ %lu ]", (void*)tmp, (long)tmp);
#endif

		stack_print_node(tmp->type, tmp->value);

		tmp = tmp->next;
		count--;
	}

printf("\n===============================End_of_Stack==============================\n\n");
}
