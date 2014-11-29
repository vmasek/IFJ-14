/**
 * @name tests_interpret.c
 * @brief Tests for interpret
 * @author Tomas Paulus (xpaulu01)
 **********************************************/

#include "tests.h"

void test_interpret()
{
	test_arithmetic_interpret();
	test_relation_functions();
}

// Tests for int and double
int test_arithmetic_interpret()
{
	int failures = 0;
	int count = 5;

	printf("*** Testing arithmetic functions in interpret ***\n");

	T_ITEM test;
	test.op1 = malloc(sizeof(int));
	test.op2 = malloc(sizeof(int));
	test.result = malloc(sizeof(int));


	test.op1->type = Type_INT;
	test.op2->type = Type_INT;

	test.op1->data.integer = 10;
	test.op2->data.integer = 20;

	test.instruction = I_ADD;

	interpret(&test);

	if (test.result->data.integer != 30)
	{
		printf("Error in a+b\n");
		failures++;
	}

	test.op1->data.integer = test.result->data.integer;
	test.op2->data.integer = 10;
	test.instruction = I_MULTIPLY;

	interpret(&test);

	if (test.result->data.integer != 300)
	{
		printf("Error in (a+b)*c == %d \n", test.result->data.integer);
		failures++;
	}

	test.op1->data.integer = test.result->data.integer;
	test.op2->data.integer = 5;
	test.instruction = I_DIV;

	interpret(&test);

	if (test.result->data.real != 60)
	{
		printf("Error in ((a+b)*c) / d == %.2f\n", test.result->data.real);
		failures++;
	}

	test.op1->type = Type_DOUBLE;
	test.op1->data.real = test.result->data.real;
	test.op2->data.integer = 12;
	test.instruction = I_SUB;

	interpret(&test);

	if (test.result->data.real != 48)
	{
		printf("Error in (((a+b)*c) / d) - e) == %.2f \n", test.result->data.real);
		failures++;
	}

	debug("Result of ((10+20) * 10) / 5) - 12 is %.2f\n", test.result->data.real);
	print_result(count, failures);

	free(test.op1);
	free(test.op2);
	free(test.result);

	return failures;
}

//no leaks but !43! errors ? :(
int test_relation_functions()
{

	printf("\n*** Testing relation less in interpret ***\n");
	int failures = 0;
	int count = 10;

	T_ITEM test;
	test.op1 = malloc(sizeof(int));
	test.op2 = malloc(sizeof(int));
	test.result = malloc(sizeof(int));


	test.op1->type = Type_INT;
	test.op2->type = Type_INT;

	test.op1->data.integer = 10;
	test.op2->data.integer = 20;

	test.instruction = I_LESS;
	interpret(&test);

//==========TEST FOR INT====================
	if (!test.result->data.boolean) // I_LESS
	{
		failures++;
		printf("Error in I_LESS 10 < 20\n");
	}

	test.instruction = I_GREATER;
	interpret(&test);

	if (test.result->data.boolean)
	{
		failures++;
		printf("Error in I_GREATER 10 < 20\n");
	}

	test.instruction = I_LESS_EQUAL;
	interpret(&test);

	if (!test.result->data.boolean)
	{
		failures++;
		printf("Error in I_LESS_EQUAL 10 < 20\n");
	}

	test.instruction = I_GREATER_EQUAL;
	interpret(&test);

	if (test.result->data.boolean)
	{
		failures++;
		printf("Error in I_GREATER_EQUAL 10 < 20\n");
	}

	test.instruction = I_EQUAL;
	interpret(&test);

	if (test.result->data.boolean)
	{
		failures++;
		printf("Error in I_EQUAL 10 < 20\n");
	}

	test.instruction = I_NOT_EQUAL;
	interpret(&test);

	if (!test.result->data.boolean)
	{
		failures++;
		printf("Error in I_NOT_EQUAL 10 < 20\n");
	}

	
	print_result(count, failures);

	free(test.op1);
	free(test.op2);
	free(test.result);

	return failures;
}

/*
** For Vojtík -> please make some tests for strings.
int test_arithmetic_interpret_string()
{

}*/