/**
 *  @name   tests_stack.c
 *  @brief  Tests for stack
 *  @author Vojtech Mašek (xmasek15)
 ****************************************************************************/

#include "tests.h"



int test_stack(void)
{
	return test_stack_complex();
}


int test_stack_complex(void)
{

	static int errors = 1;

	printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	Stack stack;
	stack_init(&stack);

	stack_print(&stack);	//This print should print nothing



	int type = Type_STRING;
	void *value = (void*)"this is char*";


	stack_push(&stack, type, (void*)value);

	stack_print(&stack);

	type = Type_STRING;
	value = (void*)"this is also char*";

	stack_push(&stack, type, (void*)value);

	stack_print(&stack);

	stack_free(&stack);
	stack_print(&stack);	//This print should print nothing

printf("\n0. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
///---------------------------------------------------------------------------
#define _N_ 12
	double	d[2]  = {0.0, 51.25};
	int		i[4]  = {42, 0, 5, -1};
	cstring *cstr = cstr_create_str("this is cstring");
	cstring *cstr1= cstr_create_str("cstr of NEW top");
	char	c	  = 'c';

	int	types[_N_]= {		Type_STRING,
							Type_INT,
							Type_INT,
							Type_DOUBLE,
							Type_CSTRING,
							Type_INT,
							99,				//valid another type
							Type_DOUBLE,
							88,				//valid another type
							Type_OTHER,
							Type_CHAR,
							Type_INT,
					};

	void *values[_N_]=	{	(void*)"just another char*",
							(void*)&i[0],
							(void*)NULL,
							(void*)&d[0],
							(void*)cstr,
							(void*)&i[1],
							(void*)"another data as string",
							(void*)&d[1],
							(void*)NULL,
							(void*)"other data as string",
							(void*)&c,
							(void*)&i[2],
						};

	for(int i=0; i<_N_; i++)
	{
		stack_push(&stack, types[i], values[i]);
	}

do{ /// do{...}while(0); 	is for testing hack

	if(stack.count==(_N_))			// (_N_) is default
		stack_print(&stack);
	else
	{
		printf("\t0. ERROR: stack counter should be %d and its %u\n", (_N_), stack.count);
		break;
	}

printf("\n1. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
///---------------------------------------------------------------------------
	int xxx = 111; ///will rewrite value so we are sure that new was grabbed correctly
	value = (void*)&xxx;

	//printf("\n1. value before top %d\n", *(int*)value);
	stack_top(&stack, &type, &value);
	//printf("\n1. value after top %d\n", *(int*)value);
	printf("\n\n\t1. reading the top of stack:");
	stack_print_node(type, value);
	if(type == Type_INT && *(int*)value == i[2])
		printf("\t1. TOP was OK.\n");
	else
	{
		printf("\t1. ERROR: TOP went wrong.\n");
		break;
	}

printf("\n2. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
///---------------------------------------------------------------------------
	///Deleting type and value leftovers.
	type = 0;
	value = (void*)NULL;

	type = Type_DOUBLE;
	stack_read_first_of_type(&stack, type, &value);
	printf("\n\n\t2. reading the first of type DOUBLE:");
	stack_print_node(type, value);

	///Deleting type and value leftovers.
	type = 0;
	value = (void*)NULL;

printf("\n3. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
///---------------------------------------------------------------------------


	stack_pop(&stack);
	if(stack.count==(_N_)-1)			// (_N_) is default so afer 1 pop its "-1"
	{
		stack_top(&stack, &type, &value);
		printf("\n\n\t3. reading the top of stack:");
		stack_print_node(type, value);

		if(type == Type_CHAR && *(char*)value == c)
			printf("\t3. TOP was OK. top is: [char]\n");
		else
		{
			printf("\t3. ERROR: POP went wrong.\n");
			break;
		}

		stack_print(&stack);
	}
	else
	{
		printf("\t3. ERROR: stack counter should be %d and its %u\n", (_N_)-1, stack.count);
		break;
	}

///*******************
	stack_pop(&stack);

	if(stack.count==(_N_)-2)			// (_N_) is default so afer 2 pops its "-2"
	{
		///readind top of stack by using direct stack access
		if(stack.top->type == Type_OTHER && (!strcmp((char*)(stack.top->value), "other data as string")))
			printf("\t3.1.1. POP was OK. top is: [other (with char* value)]\n");
		else
		{
			printf("\t3.1.1. ERROR: POP went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == Type_OTHER && (!strcmp((char*)(value), "other data as string")))
			printf("\t3.1.2. TOP was OK. top is: [other (with char* value)]\n");
		else
		{
			printf("\t3.1.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	}
	else
	{
		printf("\t3.1. ERROR: stack counter should be %d and its %u\n", (_N_)-2, stack.count);
		break;
	}
	printf("\n");

///*******************
	stack_pop(&stack);

	if(stack.count==(_N_)-3)			// (_N_) is default so afer 3 pops its "-3"
	{
		///readind top of stack by using direct stack access
		if(stack.top->type == 88 && (stack.top->value)== NULL )
			printf("\t3.2.1. POP was OK. top is: [another (88) (with NULL value)]\n");
		else
		{
			printf("\t3.2.1. ERROR: POP went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == 88 && value == NULL)
			printf("\t3.2.2. TOP was OK. top is: [another (88) (with NULL value)]\n");
		else
		{
			printf("\t3.2.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	}
	else
	{
		printf("\t3.2. ERROR: stack counter should be %d and its %u\n", (_N_)-3, stack.count);
		break;
	}
	printf("\n");
///*******************

	///PUSH
	type = Type_INT;
	value = (void*)&i[3];
	stack_push(&stack, type, value);


	if(stack.count==(_N_)-3+1)			// (_N_) is default so afer 3 pops and 1 push its "-3+1"
	{
		///readind top of stack by using direct stack access
		if(stack.top->type == Type_INT && (*(int*)(stack.top->value) == i[3]))
			printf("\t3.3.1. PUSH was OK. top is: [ int ]\n");
		else
		{
			printf("\t3.3.1. ERROR: PUSH went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == Type_INT && (*(int*)(value) == i[3]))
			printf("\t3.3.2. TOP was OK. top is: [ int ]\n");
		else
		{
			printf("\t3.3.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	}
	else
	{
		printf("\t3.3. ERROR: stack counter should be %d and its %u\n", (_N_)-3+1, stack.count);
		break;
	}
	printf("\n");
///*******************

	stack_pop(&stack);
	stack_pop(&stack);

	if(stack.count==(_N_)-5+1)			// (_N_) is default so afer 5 pops and 1 push its "-5+1"
	{
		///readind top of stack by using direct stack access
		if(stack.top->type == Type_DOUBLE && (*(double*)(stack.top->value) == d[1]))
			printf("\t3.4.1. POP was OK. top is: [ double ]\n");
		else
		{
			printf("\t3.4.1. ERROR: PUSH went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == Type_DOUBLE && (*(double*)(value) == d[1]))
			printf("\t3.4.2. TOP was OK. top is: [ double ]\n");
		else
		{
			printf("\t3.4.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	}
	else
	{
		printf("\t3.4. ERROR: stack counter should be %d and its %u\n", (_N_)-5+1, stack.count);
		break;
	}
	printf("\n");
///*******************

///STACK INSERT
	double ddd = 99.99;
	const char *string = "nejaky text";

	unsigned int old_count = stack.count;
	stack_print(&stack);
	printf("\t3.5. Inserting [ double ] behind first [ cstring ]\n");
	stack_insert(&stack, Type_CSTRING, Type_DOUBLE, (void*)&ddd);
	printf("\t3.5. Inserting [ another (90) with char* value ] behind first [ int ]\n");
	stack_insert(&stack, Type_INT, 90, (void*)string);
	stack_insert(&stack, Type_STRING, 89, (void*)NULL);

	if(old_count+3 == stack.count)
	{
		printf("\t3.5. Inserts were OK. %u nodes were inserted.\n", stack.count-old_count);
	}
	else
	{
		printf("\t3.5. ERROR in inserts were wrong.\n");
		printf("\t3.5. count should be %u and is %u.\n", old_count+2, stack.count);
		//stack_print(&stack);
		break;
	}
	///this insert shoul fail
	if(stack_insert(&stack, Type_CHAR, Type_DOUBLE, (void*)&ddd)==INTERNAL_ERROR)
		printf("\t3.6. Ret. code of failed insertion OK.");
	else
	{
		printf("\t3.6. Wrong ret. code of failed insertion.");
		break;
	}

	if(old_count+3 == stack.count)
	{
		printf("\t3.6.1 Insert failed which is OK.\n");
	}
	else
	{
		printf("\t3.6.1. Insert did not failed.\n");
		//stack_print(&stack);
		break;
	}

	stack_print(&stack);

	stack_insert(&stack, Type_INT, Type_STRING, (void*)string);

	printf("\n");

///*******************

///pop all to the last one
	printf("\t3.7. POPing all exept last two.\n");
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);
	stack_pop(&stack);

	stack_print(&stack);

	if(stack.count==(_N_)-15+2+3)			// (_N_) is default so afer 13 pops and 2 pushes and 3 inserts its "--13+3"
	{
		///readind top of stack by using direct stack access
		if(stack.top->type == Type_STRING && (!strcmp((char*)(stack.top->value), "just another char*")) )
			printf("\t3.7.1. POP was OK. top is: [ char* ]\n");
		else
		{
			printf("\t3.7.1. ERROR: POP went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == Type_STRING && (!strcmp((char*)(value), "just another char*")))
			printf("\t3.7.2. TOP was OK. top is: [ char* ]\n");
		else
		{
			printf("\t3.7.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	}
	else
	{
		printf("\t3.7. ERROR: stack counter should be %d and its %u\n", (_N_)-15+2+3, stack.count);
		break;
	}

	printf("\t3.8. POPing the last node in stack\n");
	stack_pop(&stack);
	stack_pop(&stack);

	stack_print(&stack);

	if(stack.count==0)
		printf("\t3.8.1. POP was OK. Stack is empty.");
	else
	{
		printf("\t3.8.1. POP went wrong. Stack is not empty.\n");
		break;
	}
	if(stack.top == NULL)
		printf(" Top is NULL.\n");
	else
	{
		printf(" But top is not NULL !!!\n");
		break;
	}


printf("\n4. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	///Setting type and value to cstring.
	type = Type_CSTRING;
	value = (void*)cstr1;
	stack_push(&stack, type, value); 						///pushin cstring
	///Deleting type and value leftover after assigning cstring.
	type = 0;
	value = (void*)NULL;
	///checking new top (should be cstring)
	stack_top(&stack, &type, &value);
	printf("\n\n\t4. reading the NEW top of the stack:\n");
	///readind top of stack by using direct stack access
		if(stack.top->type == Type_CSTRING && (!strcmp(((cstring*)(stack.top->value))->str, "cstr of NEW top")))
			printf("\t4.1. PUSH was OK. top is: [cstring]\n");
		else
		{
			printf("\t4.1. ERROR: PUSH went wrong., type was %d and value was %s\n", stack.top->type, (char*)(stack.top->value));
			break;
		}

		///readind top of stack by using stack_top
		stack_top(&stack, &type, &value);

		if(type == Type_CSTRING && (!strcmp(((cstring*)(stack.top->value))->str, "cstr of NEW top")))
			printf("\t4.1. TOP was OK. top is: [cstring]\n");
		else
		{
			printf("\t4.2. ERROR: TOP went wrong., type was %d and value was %s\n", type, (char*)(value));
			break;
		}
	stack_print_node(type, value);
	stack_print(&stack);


printf("\n5. ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
///---------------------------------------------------------------------------

errors=0;

}while(0);

	stack_free(&stack);
	stack_print(&stack);	//This print should print nothing

	cstr_free(cstr1);
	cstr_free(cstr);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

	return errors;
}