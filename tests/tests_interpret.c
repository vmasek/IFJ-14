/**
 * @file tests_interpret.c
 * @brief Tests for interpreter instructions
 * @author Vojtech Mašek (xmasek15), Tomáš Paulus (xpaulu01)
 ************************************************************/


#include "tests.h"

void test_I_ASSIGN(void);			///fine  //uninit is not handled // passed NULL values as stack were sigfaulting

void test_I_ADD(void);				///fine

void test_I_SUB(void);				///fine

void test_I_MUL(void);				///fine

void test_I_DIV(void);				///fine  //div by zero ok

void test_I_LESS(void);				///fine

void test_I_GREATER(void);			///fine

void test_I_LESS_EQUAL(void); 		///fine

void test_I_GREATER_EQUAL(void);	//fine

void test_I_EQUAL(void);			//fine

void test_I_NOT_EQUAL(void);		//fine

void test_interpreter(int argc, char* argv)
{
	if(!argc || !strcmp(argv, "I_ASSIGN" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_ASSIGN();
	}
	if(!argc || !strcmp(argv, "I_ADD" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_ADD();
	}
	if(!argc || !strcmp(argv, "I_SUB" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_SUB();
	}
	if(!argc || !strcmp(argv, "I_MUL" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_MUL();
	}
	if(!argc || !strcmp(argv, "I_DIV" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_DIV();
	}
	if(!argc || !strcmp(argv, "I_LESS" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_LESS();
	}
	if(!argc || !strcmp(argv, "I_GREATER" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_GREATER();
	}
	if(!argc || !strcmp(argv, "I_LESS_EQUAL" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_LESS_EQUAL();
	}
	if(!argc || !strcmp(argv, "I_GREATER_EQUAL" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_GREATER_EQUAL();
	}
	if(!argc || !strcmp(argv, "I_EQUAL" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_EQUAL();
	}
	if(!argc || !strcmp(argv, "I_NOT_EQUAL" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_NOT_EQUAL();
	}
}



void test_I_ASSIGN(void)
{
	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_ASSIGN;


///-----Variables globals--------------
	Variables globals;
	variables_init(&globals);


///-----Stack locals--------------
	Stack locals;
	stack_init(&locals);

///-----Stack calcs---------------
	Stack calcs;
	stack_init(&calcs);

	Value values[4];

	///tu je potrebne si popridavat values pre testy

	bool b = true;
	double d = 1.5;
	cstring *cstr = cstr_create_str("globals text");

	values[0].integer	= 42;
	values[1].real		= 16.125;
	values[2].boolean	= 1;
	values[3].string	= cstr_create_str("text");
	values[4].integer	= 1234;

	variables_add(&globals, TYPE_INT, values[0], NULL);

	stack_push(&locals, TYPE_REAL, &(d));
	stack_push(&locals, TYPE_BOOL, &(b));
	stack_push(&locals, TYPE_STRING, cstr);
	stack_print(&locals);
///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na assign

	instruction.index		= 0;									 ///bude zapis do globals index 0

printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	if(stack_push(&calcs, TYPE_INT, &(values[test].integer)) == INTERNAL_ERROR)
	{
		debug("stack push sa dorafal !");
		break;
	}
/*
	if(interpret(&instruction, &calcs, NULL, NULL, NULL) == INTERNAL_ERROR)
	{
		printf("\n%s:[test -1.] OK: NULL variables test.\n\n", __func__);
	}
	else
	{
		printf("\n%s:[test -1.] ERROR: NULL variables test.\n\n", __func__);
		break;
	}*/

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n\n", __func__, test);
		break;
	}


	if(calcs.count==0 && globals.count==1)
	{
		///readind top of stack by using direct stack access
		if(globals.types[0] == TYPE_INT && globals.values[0].integer == values[test].integer)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[test].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tglobals should be %d and its %u\n", __func__, test, 0, calcs.count, 1, globals.count);
		break;
	}
	printf("\n");


///DRUHY test na assign
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	instruction.index		= -3;									 ///bude zapis do locals index 2 "( -(index+1) )"

	stack_push(&calcs, TYPE_REAL, &(values[test].real));

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		///readind top of stack by using direct stack access
		if(locals.top->next->next->type == TYPE_REAL && (*(double*)(locals.top->next->next->value) == values[test].real))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(locals.top->next->next->value), values[test].real);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 0, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");


///TRETI test na assign
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

instruction.index		= -2;

	stack_push(&calcs, TYPE_BOOL, &(values[test].boolean));

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		///readind top of stack by using direct stack access
		if(locals.top->next->type == TYPE_BOOL && (*(bool*)(locals.top->next->value) == values[test].boolean))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is (bool)%d and should be (bool)%d\n", __func__, test, *(bool*)(locals.top->next->value), values[test].boolean);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 0, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");



///STVRTY test na assign
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

instruction.index		= -1;
	stack_print(&locals);
	stack_push(&calcs, TYPE_STRING, values[test].string);

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		///readind top of stack by using direct stack access
		if(locals.top->type == TYPE_STRING && !cstr_cmp((cstring*)(locals.top->value), values[test].string))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is (cstring) \"%s\" and should be (cstring) \"%s\"\n", __func__, test, ((cstring*)(locals.top->value))->str, values[test].string->str);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 0, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");

/*
///PIATY test na assign
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

///toto bude test na neinicializovanu premennu
///pushnem do locals stacku TYPE_OTHER ktory znaci neinicializovenie a null pointer
stack_push(&locals, TYPE_OTHER, NULL);
instruction.index		= -1;
stack_print(&locals);


	stack_push(&calcs, TYPE_INT, &(values[test].integer));

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==4)
	{
		///readind top of stack by using direct stack access
		if(locals.top->type == TYPE_INT && (*(int*)(locals.top->value) == values[test].integer))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(locals.top->value), values[test].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 0, calcs.count, 4, locals.count);
		break;
	}
	printf("\n");
*/

END_OF_TEST();

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
	values[4].real 		= 8.75;
	values[5].string	= cstr_create_str("text1");
	values[6].string	= cstr_create_str("text2");

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na add


printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme scitat
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer + values[1].integer )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer + values[1].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///DRUHY test na add
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real + values[4].real )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///TRETI test na add
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer + values[4].real )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer + values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///STVRTY test na add
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real + values[1].integer )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real + values[1].integer));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///PIATY test na add
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_STRING, values[5].string);
	stack_push(&calcs, TYPE_STRING, values[6].string); ///ten isty 5 potom dat na 6!!!

	///zavolanie interpretera
	CALL_INTERPRET();

	stack_print(&calcs);

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_STRING && ( !cstr_cmp_str((cstring*)(calcs.top->value), "text1text2") ) )
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %s and should be %s\n", __func__, test, ((cstring*)(calcs.top->value))->str, "text1text2");
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



END_OF_TEST();

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
	values[4].real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na sub
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme odcitat
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer - values[1].integer )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer - values[1].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///DRUHY test na sub
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real - values[4].real )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///TRETI test na sub
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer - values[4].real )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer - values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///STVRTY test na sub
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real - values[1].integer )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real - values[1].integer));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();

END_OF_TEST();

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
	values[4].real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na mul
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme nasobit
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer * values[1].integer )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer * values[1].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///DRUHY test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real * values[4].real )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real * values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///TRETI test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer * values[4].real )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer * values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();



///STVRTY test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real * values[1].integer )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real * values[1].integer));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();

END_OF_TEST();

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
	values[0].integer	= 2;
	values[1].integer	= 100;

	values[2].real		= 16.125;
	values[3].real		= 2.5;
	values[4].real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na mul
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme delit
	stack_push(&calcs, TYPE_INT, &(values[0].integer));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_INT && (*(int*)(calcs.top->value) == ( values[0].integer / values[1].integer )))
			printf("\n%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, *(int*)(calcs.top->value), values[0].integer / values[1].integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///DRUHY test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real / values[4].real )))
			printf("\n%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real / values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///TRETI test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_INT, &(values[1].integer));
	stack_push(&calcs, TYPE_REAL, &(values[4].real));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[1].integer / values[4].real )))
			printf("\n%s:[test %d.] DIV int / real was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[1].integer / values[4].real));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();


///STVRTY test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_INT, &(values[1].integer));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		///readind top of stack by using direct stack access
		if(calcs.top->type == TYPE_REAL && (*(double*)(calcs.top->value) == ( values[3].real / values[1].integer )))
			printf("\n%s:[test %d.] DIV real / int was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, *(double*)(calcs.top->value), (values[3].real / values[1].integer));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");

	///v stacku je iba jedna vec (result operacie assign) tak ho popneme a zistime ci je stack prazdny
	POP_OUT_LAST();

///PIATY test na mul
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

Value nula = { .real=0.0 };

	stack_push(&calcs, TYPE_REAL, &(values[3].real));
	stack_push(&calcs, TYPE_REAL, &(nula.real));

	///zavolanie interpretera na delenie nulou
	if(interpret(&instruction, &calcs, NULL, NULL, NULL) != RUNTIME_DIV_BY_ZERO)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}
	else
	{
		printf("\n%s:[test %d.] OK: Dividin by zero returned correct code.\n", __func__, test);
	}


END_OF_TEST();


}


void test_I_LESS(void)
{

TEST_OF_COMPARE("Less", < , I_LESS);

}


void test_I_GREATER(void)
{

TEST_OF_COMPARE("Greather", >, I_GREATER);

}


void test_I_LESS_EQUAL(void)
{

TEST_OF_COMPARE("Less_equal", <= , I_LESS_EQUAL);

}


void test_I_GREATER_EQUAL(void)
{

TEST_OF_COMPARE("Greather_equal", >= , I_GREATER_EQUAL);

}


void test_I_EQUAL(void)
{

TEST_OF_COMPARE("Equal", == , I_EQUAL);

}


void test_I_NOT_EQUAL(void)
{

TEST_OF_COMPARE("Not_equal", != , I_NOT_EQUAL);

}
