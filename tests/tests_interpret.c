/**
 * @file tests_interpret.c
 * @brief Tests for interpreter instructions
 * @author Vojtech Mašek (xmasek15), Tomáš Paulus (xpaulu01)
 ************************************************************/


#include "tests.h"

void test_I_ASSIGN(void);			///fine  //uninit is not handled // passed NULL values as stack were sigfaulting

void test_I_PUSH(void);				//

void test_I_ADD(void);				///fine

void test_I_SUB(void);				///fine

void test_I_MUL(void);				///fine

void test_I_DIV(void);				///fine  //div by zero ok

void test_I_LESS(void);				///fine

void test_I_GREATER(void);			///fine

void test_I_LESS_EQUAL(void); 		///fine

void test_I_GREATER_EQUAL(void);	///fine

void test_I_EQUAL(void);			///fine

void test_I_NOT_EQUAL(void);		///fine

void test_I_AND(void);

void test_I_OR(void);

void test_I_XOR(void);

void test_I_NOT(void);


void test_interpreter(int argc, char* argv)
{
	if(!argc || !strcmp(argv, "I_ASSIGN" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_ASSIGN();
	}
	if(!argc || !strcmp(argv, "I_PUSH" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_PUSH();
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
	if(!argc || !strcmp(argv, "I_AND" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_AND();
	}
	if(!argc || !strcmp(argv, "I_OR" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_OR();
	}
	if(!argc || !strcmp(argv, "I_XOR" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_XOR();
	}
	if(!argc || !strcmp(argv, "I_NOT" ))
	{
		printf("\n=================================================================================================================\n");
		test_I_NOT();
	}
}



void test_I_ASSIGN(void)
{
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_ASSIGN;


///-----Variables globals--------------
	Variables globals;
	variables_init(&globals);


///-----Stack locals--------------
	Stack locals;
	stack_init(&locals, VALUE_STACK);

///-----Stack calcs---------------
	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy


	values[0].data.integer	= 42;
	values[1].data.real		= 16.125;
	values[2].data.boolean	= 1;
	values[3].data.string	= cstr_create_str("text");
	values[4].data.integer	= 1234;

	values[7].data.boolean   = true;
	values[8].data.real      = 1.5;
	values[9].data.string    = cstr_create_str("globals text");

	variables_add(&globals, TYPE_INT, values[0], NULL);
	stack_push(&locals, TYPE_REAL, &(values[8]));
	stack_push(&locals, TYPE_BOOL, &(values[7]));
	stack_push(&locals, TYPE_STRING, &values[9]);
	stack_print(&locals);
///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na assign

	instruction.index		= 0;									 ///bude zapis do globals index 0

printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	if(stack_push(&calcs, TYPE_INT, &(values[test])) == INTERNAL_ERROR)
	{
		debug("stack push sa dorafal !");
		break;
	}

	if(interpret(&instruction, &calcs, NULL, NULL, NULL) == INTERNAL_ERROR)
	{
		printf("\n%s:[test -1.] OK: NULL variables test.\n\n", __func__);
	}
	else
	{
		printf("\n%s:[test -1.] ERROR: NULL variables test.\n\n", __func__);
		break;
	}

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n\n", __func__, test);
		break;
	}


	if(calcs.count==0 && globals.count==1)
	{
		if(globals.types[0] == TYPE_INT && globals.values[0].data.integer == values[test].data.integer)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			Value temp;
			variables_value_read(&globals, NULL, &temp, 0);
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[test].data.integer);
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

	stack_push(&calcs, TYPE_REAL, &(values[test]));

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		stack_index(&locals, 2, (int*)&typ, &temp);
		if(typ == TYPE_REAL && temp.data.real == values[test].data.real)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, values[test].data.real);
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

	stack_push(&calcs, TYPE_BOOL, &(values[test]));

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		stack_index(&locals, 1, (int*)&typ, &temp);
		if(typ == TYPE_BOOL && temp.data.boolean == values[test].data.boolean)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is (bool)%d and should be (bool)%d\n", __func__, test, temp.data.boolean, values[test].data.boolean);
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
	stack_push(&calcs, TYPE_STRING, &values[test]);

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	stack_print(&locals);

	if(calcs.count==0 && locals.count==3)
	{
		stack_index(&locals, 0, (int*)&typ, &temp);
		if(typ == TYPE_STRING && !cstr_cmp(temp.data.string, values[test].data.string))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is (cstring) \"%s\" and should be (cstring) \"%s\"\n", __func__, test, temp.data.string->str, values[test].data.string->str);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 0, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");

END_OF_TEST();

stack_free(&locals);
variables_free(&globals);

}


void test_I_PUSH(void)
{
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_PUSH;


///-----Variables globals--------------
	Variables globals;
	variables_init(&globals);


///-----Stack locals--------------
	Stack locals;
	stack_init(&locals, VALUE_STACK);

///-----Stack calcs---------------
	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy


	values[0].data.integer	= 42;
	values[0].inited = true;

	values[1].data.real		= 16.125;
	values[2].data.boolean	= 1;
	values[3].data.string	= cstr_create_str("text");
	values[4].data.integer	= 1234;

	values[7].data.boolean   = true;
	values[7].inited = true;
	values[8].data.real      = 1.5;
	values[8].inited = true;
	values[9].data.string    = cstr_create_str("globals text");
	values[9].inited = true;

	variables_add(&globals, TYPE_INT, values[0], NULL);

	stack_push(&locals, TYPE_REAL, &(values[8]));
	stack_push(&locals, TYPE_BOOL, &(values[7]));
	stack_push(&locals, TYPE_STRING, &values[9]);
	stack_print(&locals);
///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na push

	instruction.index		= -1;									 ///bude push z locals index 0 (-(-1+1))

printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n\n", __func__, test);
		break;
	}


	if(calcs.count==1 && locals.count==3)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_STRING && !cstr_cmp(temp.data.string, values[9].data.string))
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is (cstring) \"%s\" and should be (cstring) \"%s\"\n", __func__, test, temp.data.string->str, values[9].data.string->str);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 1, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");


///DRUHY test na push
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);


	instruction.index		= -2;

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	if(calcs.count==2 && locals.count==3)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_BOOL && temp.data.boolean == values[7].data.boolean)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be (bool) %d\n", __func__, test, temp.data.boolean, values[7].data.boolean);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 1, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");


///TRETI test na push
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

instruction.index		= -3;

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	if(calcs.count==3 && locals.count==3)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && temp.data.real == values[8].data.real)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, values[8].data.real);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 3, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");



///STVRTY test na push
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

	instruction.index		= 0; ///zoberie z globals index 0 (prve)

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	if(calcs.count==4 && locals.count==3 && globals.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && temp.data.integer == values[0].data.integer)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 4, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");


///PIATY test na push (zavola instrukciu ADD nad pushnutymi vecami)
test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);

instruction.index		= 0; ///toto je momentalne nepotrebne
instruction.instruction		= I_ADD;

	if(interpret(&instruction, &calcs, &locals, NULL, &globals) != SUCCESS)
	{
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);
		break;
	}

	if(calcs.count==3 && locals.count==3 && globals.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && temp.data.real == values[8].data.real+values[0].data.integer)
			printf("\n%s:[test %d.] ASSIGN was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, values[8].data.real+values[0].data.integer);
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: calcs should be %d and its %u\tlocals should be %d and its %u\n", __func__, test, 3, calcs.count, 3, locals.count);
		break;
	}
	printf("\n");

END_OF_TEST();

stack_free(&locals);
variables_free(&globals);

}


void test_I_ADD(void)
{
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_ADD;

	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].data.integer	= 8;
	values[1].data.integer	= 100;

	values[2].data.real		= 16.125;
	values[3].data.real		= 2.5;
	values[4].data.real 		= 8.75;
	values[5].data.string	= cstr_create_str("text1");
	values[6].data.string	= cstr_create_str("text2");

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na add


printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme scitat
	stack_push(&calcs, TYPE_INT, &(values[0]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( values[0].data.integer + values[1].data.integer )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer + values[1].data.integer);
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


	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real + values[4].data.real )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real - values[4].data.real));
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


	stack_push(&calcs, TYPE_INT, &(values[1]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[1].data.integer + values[4].data.real )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[1].data.integer + values[4].data.real));
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

	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real + values[1].data.integer )))
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real + values[1].data.integer));
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

	stack_push(&calcs, TYPE_STRING, &values[5]);
	stack_push(&calcs, TYPE_STRING, &values[6]); ///ten isty 5 potom dat na 6!!!

	///zavolanie interpretera
	CALL_INTERPRET();

	stack_print(&calcs);

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_STRING && ( !cstr_cmp_str(temp.data.string, "text1text2") ) )
			printf("\n%s:[test %d.] ADD was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %s and should be %s\n", __func__, test, (temp.data.string)->str, "text1text2");
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
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_SUB;

	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].data.integer	= 8;
	values[1].data.integer	= 100;

	values[2].data.real		= 16.125;
	values[3].data.real		= 2.5;
	values[4].data.real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na sub
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme odcitat
	stack_push(&calcs, TYPE_INT, &(values[0]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( values[0].data.integer - values[1].data.integer )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer - values[1].data.integer);
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


	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real - values[4].data.real )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real - values[4].data.real));
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


	stack_push(&calcs, TYPE_INT, &(values[1]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[1].data.integer - values[4].data.real )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[1].data.integer - values[4].data.real));
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

	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real - values[1].data.integer )))
			printf("\n%s:[test %d.] SUB was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real - values[1].data.integer));
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
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_MUL;

	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].data.integer	= 8;
	values[1].data.integer	= 100;

	values[2].data.real		= 16.125;
	values[3].data.real		= 2.5;
	values[4].data.real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na mul
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme nasobit
	stack_push(&calcs, TYPE_INT, &(values[0]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( values[0].data.integer * values[1].data.integer )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer * values[1].data.integer);
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


	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real * values[4].data.real )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real * values[4].data.real));
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


	stack_push(&calcs, TYPE_INT, &(values[1]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[1].data.integer * values[4].data.real )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[1].data.integer * values[4].data.real));
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

	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real * values[1].data.integer )))
			printf("\n%s:[test %d.] MUL was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real * values[1].data.integer));
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
	Type typ;
	Value temp;

	int test = 0, errors = 1;

///-----declarations-of-needed-structs----------------------------------------
	Instruction instruction;
	instruction.instruction = I_DIV;

	Stack calcs;
	stack_init(&calcs, VALUE_STACK);

	Value values[10];

	///tu je potrebne si popridavat values pre testy
	values[0].data.integer	= 2;
	values[1].data.integer	= 100;

	values[2].data.real		= 16.125;
	values[3].data.real		= 2.5;
	values[4].data.real 		= 8.75;

///---------------------------------------------------------------------------

BEGINE_OF_TEST();


///PRVY test na mul
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	///push dvoch int values ktore budeme delit
	stack_push(&calcs, TYPE_INT, &(values[0]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( values[0].data.integer / values[1].data.integer )))
			printf("\n%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer / values[1].data.integer);
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


	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real / values[4].data.real )))
			printf("\n%s:[test %d.] DIV was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real / values[4].data.real));
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


	stack_push(&calcs, TYPE_INT, &(values[1]));
	stack_push(&calcs, TYPE_REAL, &(values[4]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[1].data.integer / values[4].data.real )))
			printf("\n%s:[test %d.] DIV int / real was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[1].data.integer / values[4].data.real));
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


	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_INT, &(values[1]));

	///zavolanie interpretera
	CALL_INTERPRET();

	///kontrola velkosti stacku, boli pushnute dve hodnoty, tie su v inter. popnute a je pushnuty result
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_REAL && (temp.data.real == ( values[3].data.real / values[1].data.integer )))
			printf("\n%s:[test %d.] DIV real / int was OK.\n", __func__, test);
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %f and should be %f\n", __func__, test, temp.data.real, (values[3].data.real / values[1].data.integer));
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

Value nula = { .data.real=0.0 };

	stack_push(&calcs, TYPE_REAL, &(values[3]));
	stack_push(&calcs, TYPE_REAL, &(nula));

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

void test_I_AND(void)
{

TEST_OF_BIT_OPERATORS("And", & , I_AND);

}

void test_I_OR(void)
{

TEST_OF_BIT_OPERATORS("Or", | , I_OR);

}

void test_I_XOR(void)
{

TEST_OF_BIT_OPERATORS("Xor", ^ , I_XOR);

}

void test_I_NOT(void)
{

	Type typ;
	Value temp;
	int test = 0, errors = 1;
	Instruction instruction;
	instruction.instruction = I_NOT;
	Stack calcs;
	stack_init(&calcs, VALUE_STACK);
	Value values[6];
	values[0].data.integer	= 8;
	values[1].data.integer	= 100;
	values[2].data.integer	= 0;
	values[3].data.integer	= -1;
	values[4].data.integer	= -54646406;
	values[5].data.integer	= 2154668;

BEGINE_OF_TEST();


printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[0]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
	stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[0].data.integer )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, ~values[0].data.integer );
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[1]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
	stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[1].data.integer )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, ( ~values[1].data.integer ));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[5]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
		stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[5].data.integer  )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is  %d and should be %d\n", __func__, test, temp.data.integer, ( ~values[5].data.integer ));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[4]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
	stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[4].data.integer  )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, ( ~values[4].data.integer ));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[3]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
	stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[3].data.integer  )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, ( ~values[3].data.integer ));
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


test++;
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);
	stack_push(&calcs, TYPE_INT, &(values[2]));

	CALL_INTERPRET();
	if(calcs.count==1)
	{
	stack_index(&calcs, 0, (int*)&typ, &temp);
		if(typ == TYPE_INT && (temp.data.integer == ( ~values[2].data.integer )))
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, "NOT");
		else
		{
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, ~values[2].data.integer );
			break;
		}
	}
	else
	{
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);
		break;
	}
	printf("\n");
	POP_OUT_LAST();


END_OF_TEST();

}

