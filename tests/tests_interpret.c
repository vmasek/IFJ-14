/**
 * @file tests_interpret.c
 * @brief Tests for interpreter instructions
 * @author Vojtech Mašek (xmasek15), Tomáš Paulus (xpaulu01)
 ************************************************************/

//Pozn.: Make tests also for bad numbers

#include "tests.h"

void test_I_ASSIGN(void);			//fine

void test_I_ADD(void);				//fine

void test_I_SUB(void);				// int - real and real - int is not working

void test_I_MUL(void);				//fine

void test_I_DIV(void);				//fine !! do not forget that indexes are changed

void test_I_LESS(void);				//fine

void test_I_GREATER(void);			//fine

void test_I_LESS_EQUAL(void); 		//fine

void test_I_GREATER_EQUAL(void);	//fine

void test_I_EQUAL(void);			//fine

void test_I_NOT_EQUAL(void);		//fine

void test_interpreter()
{
	test_I_ASSIGN();
	printf("\n=================================================================================================================\n");
	test_I_ADD();
	printf("\n=================================================================================================================\n");
	test_I_SUB();
	printf("\n=================================================================================================================\n");
	test_I_MUL();
	printf("\n=================================================================================================================\n");
	test_I_DIV();
	printf("\n=================================================================================================================\n");
	test_I_LESS();
	printf("\n=================================================================================================================\n");
	test_I_GREATER();
	printf("\n=================================================================================================================\n");
	test_I_LESS_EQUAL();
	printf("\n=================================================================================================================\n");
	test_I_GREATER_EQUAL();
	printf("\n=================================================================================================================\n");
	test_I_EQUAL();
	printf("\n=================================================================================================================\n");
	test_I_NOT_EQUAL();
}




void test_I_ASSIGN(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_ASSIGN;

	Stack calcs;
	stack_init(&calcs);

	Value values[4];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 42;
	values[1].real		= 16.125;
	values[2].boolean	= 1;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na assign


printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[test].integer));


	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}


	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == values[test].integer))
			printf("%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[test].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na assign
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_REAL, &(values[test].real));

	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}
	printf("\n\t%d\n", calcs.count);

	///zas bol stack pred pushom prazdny takze by count mal byt 1
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == values[test].real))
			printf("%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), values[test].real);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///tentokrat uz stack nepopnem aby som otestoval ci to funguje aj s neprazdnym stackom

///TRETI test na assign
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_BOOL, &(values[test].boolean));

	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///stack pred pushom obsahoval result z predchadzajuceho + bol push takze 2
	if(calcs.count==2)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == values[test].boolean))
			printf("%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is (bool)%d and should be (bool)%d\n", __func__, test, *(bool*)(calcs.top->value), values[test].boolean);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 2, calcs.count);
		break;
	}
	printf("\n");

	///tentokrat uz stack nepopnem aby som otestoval ci to funguje aj s neprazdnym stackom


///STVRTY test na assign
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

///TODO: spravim este cstring






errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_ADD(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_ADD;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na add


printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme scitat
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer + values[1].integer )))
			printf("%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer + values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na add
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real + values[4].real )))
			printf("%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///TRETI test na add
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer + values[4].real )))
			printf("%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer + values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///STVRTY test na add
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real + values[1].integer )))
			printf("%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real + values[1].integer));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

///PIATY test na add
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
//TODO: spravim este cstring






errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}

void test_I_SUB(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_SUB;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na sub
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme odcitat
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer - values[1].integer )))
			printf("%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer - values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na sub
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real - values[4].real )))
			printf("%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///TRETI test na sub
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer - values[4].real )))
			printf("%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer - values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///STVRTY test na sub
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real - values[1].integer )))
			printf("%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[1].integer));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}

void test_I_MUL(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_MUL;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na mul
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme nasobit
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer * values[1].integer )))
			printf("%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer * values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na mul
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real * values[4].real )))
			printf("%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real * values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///TRETI test na mul
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer * values[4].real )))
			printf("%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer * values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}



///STVRTY test na mul
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real * values[1].integer )))
			printf("%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real * values[1].integer));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_DIV(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_DIV;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na mul
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme delit
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer / values[1].integer )))
			printf("%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer / values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na mul
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real / values[4].real )))
			printf("%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real / values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_LESS(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_LESS;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na less
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer < values[1].integer )))
			printf("%s:[test %d.] Less was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer < values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na less
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real < values[4].real )))
			printf("%s:[test %d.] Less was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real < values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_GREATER(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_GREATER;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na greater
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_INT, &(values[0].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer > values[1].integer )))
			printf("%s:[test %d.] Greater was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer > values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na greater
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[4].real)); // second operand was pushed first
	stack_push(&calcs, TYPE_REAL, &(values[3].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real > values[4].real )))
			printf("%s:[test %d.] Greater was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real > values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_LESS_EQUAL(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_LESS_EQUAL;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na Less-equal
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_INT, &(values[0].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer <= values[1].integer )))
			printf("%s:[test %d.] Less - equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer <= values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na Less-equal
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[4].real)); // second operand was pushed first
	stack_push(&calcs, TYPE_REAL, &(values[3].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real <= values[4].real )))
			printf("%s:[test %d.] Less - equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real <= values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_GREATER_EQUAL(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_GREATER_EQUAL;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na Greater-equal
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_INT, &(values[0].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer >= values[1].integer )))
			printf("%s:[test %d.] Greater - equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer >= values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na Greater-equal
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[4].real)); // second operand was pushed first
	stack_push(&calcs, TYPE_REAL, &(values[3].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real >= values[4].real )))
			printf("%s:[test %d.] Greater - equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real >= values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_EQUAL(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_EQUAL;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na Greater-equal
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_INT, &(values[0].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer == values[1].integer )))
			printf("%s:[test %d.] Equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer == values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na EQUAL
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[4].real)); // second operand was pushed first
	stack_push(&calcs, TYPE_REAL, &(values[3].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real == values[4].real )))
			printf("%s:[test %d.] Equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real == values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}


void test_I_NOT_EQUAL(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_NOT_EQUAL;

	Stack calcs;
	stack_init(&calcs);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].integer	= 8;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;

///---------------------------------------------------------------------------

do{ /// do{...}while(0); 	is for testing hack


///PRVY test na NOT-EQUAL
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme porovnavat
	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_INT, &(values[0].integer));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[0].integer != values[1].integer )))
			printf("%s:[test %d.] Not - Equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), values[0].integer != values[1].integer);
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}


///DRUHY test na Greater-equal
test++;
printf("\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[4].real)); // second operand was pushed first
	stack_push(&calcs, TYPE_REAL, &(values[3].real));

	///zavolanie interpretera
	if(interpret(&instruction, &calcs, NULL, NULL) != SUCCESS)
	{
		printf("%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_BOOL && (*(bool*)(calcs.top->value) == ( values[3].real != values[4].real )))
			printf("%s:[test %d.] Not - Equal was OK.\n", __func__, test);
		else
		{
			printf("%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(bool*)(calcs.top->value), (values[3].real != values[4].real));
			break;
		}
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	stack_pop(&calcs);
	if(calcs.count==0)
	{
		printf("%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);
	}
	else
	{
		printf("%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);
		break;
	}

errors=0;
}while(0);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");
	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}

}