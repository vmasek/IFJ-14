/**
 *  @name   tests.h
 *  @brief  Tests (header)
 ****************************************************************************/

#ifndef __TESTS_H__
#define __TESTS_H__

#include <string.h>
#include <stdio.h>

#include "../gc.h"
#include "../errors.h"
#include "../cstring.h"
#include "../stack.h"
#include "../ial.h"
#include "../buildin.h"
#include "../common.h"
#include "../interpreter.h"
#include "../scaner.h"


///=================================BUILDIN===================================
int test_length(void);
int test_copy(void);
int test_find(void);
int test_sort(void);
void test_buildin(void);


///=================================COMMON====================================
void print_result(int, int);


///==================================IAL======================================
int test_kmp_substr(void);


///=================================STACK=====================================
int test_stack(void);
int test_stack_complex(void);


///==================================TREE=====================================
int test_tree(void);
int test_tree_complex(void);


///=================================CSTRING===================================
int test_cstring(void);
int test_cstring_complex(void);


///===================================GC======================================
int test_gc(void);
int test_gc_complex(void);

///=================================Unary Scanner==============================
void test_scanner_unary(void);


///=================================INTERPRET=================================
void test_interpreter(int argc, char *argv);













#define CALL_INTERPRET()                                                      \
	if(interpret(&instruction, &calcs, NULL, NULL, NULL) != SUCCESS)          \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: Interpreter ended with bad error code.\n", __func__, test);\
		break;                                                                \
	}                                                                         \


#define POP_OUT_LAST()                                                        \
	stack_pop(&calcs);                                                        \
	if(calcs.count==0)                                                        \
	{                                                                         \
		printf("\n%s:[test %d.] OK. pop was ok and stack is empty\n", __func__, test);\
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 0, calcs.count);\
		break;                                                                \
	}                                                                         \




#define BEGINE_OF_TEST()                                                      \
do{                                                                           \



#define END_OF_TEST()                                                         \
errors=0;                                                                     \
}while(0);                                                                    \
                                                                              \
    stack_free(&calcs);                                                       \
                                                                              \
	if(errors)                                                                \
	{                                                                         \
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");           \
		exit(errors);                                                         \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n\nOK. Everything is fine.\n\n");                            \
	}                                                                         \



#define TEST_OF_COMPARE(operation, operator, instruct)                        \
	Type typ;                                                                 \
	Value temp;                                                               \
	int test = 0, errors = 1;                                                 \
	Instruction instruction;                                                  \
	instruction.instruction = instruct;                                       \
	Stack calcs;                                                              \
	stack_init(&calcs, VALUE_STACK);                                          \
	Value values[12];                                                         \
	values[0].data.integer	= 8;                                                    \
	values[1].data.integer	= 100;                                                  \
	values[2].data.real		= 16.125;                                                 \
	values[3].data.real		= 2.5;                                                    \
	values[4].data.real 		= 8.75;                                                  \
	values[5].data.real		= 2.5;                                                    \
	values[5].data.boolean	= true;                                                 \
	values[6].data.boolean	= false;                                                \
	values[7].data.boolean	= true;                                                 \
	values[8].data.boolean	= false;                                                \
	values[9].data.string	= cstr_create_str("text1");                              \
	values[10].data.string	= cstr_create_str("text1");                        \
	values[11].data.string	= cstr_create_str("text2");                        \
BEGINE_OF_TEST();                                                             \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_INT, &(values[0]));                       \
	stack_push(&calcs, TYPE_INT, &(values[1]));                       \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[0].data.integer operator values[1].data.integer )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, values[0].data.integer operator values[1].data.integer);\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_REAL, &(values[3]));                         \
	stack_push(&calcs, TYPE_REAL, &(values[4]));                         \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[3].data.real operator values[4].data.real )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (values[3].data.real operator values[4].data.real));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_BOOL, &(values[5]));                      \
	stack_push(&calcs, TYPE_BOOL, &(values[6]));                      \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
		stack_index(&calcs, 0, (int*)&typ, &temp);                            \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[5].data.boolean operator values[6].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (values[5].data.boolean operator values[6].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_BOOL, &(values[5]));                      \
	stack_push(&calcs, TYPE_BOOL, &(values[7]));                      \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[5].data.boolean operator values[7].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (values[5].data.boolean operator values[7].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_REAL, &(values[3]));                         \
	stack_push(&calcs, TYPE_REAL, &(values[5]));                         \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[3].data.boolean operator values[5].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (values[3].data.boolean operator values[5].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_REAL, &(values[3]));                         \
	stack_push(&calcs, TYPE_REAL, &(values[5]));                         \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[3].data.boolean operator values[5].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (values[3].data.boolean operator values[5].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
	test++;                                                                   \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_STRING, &values[9]);                        \
	stack_push(&calcs, TYPE_STRING, &values[10]);                       \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == (cstr_cmp(values[9].data.string, values[10].data.string) operator 0) ))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (cstr_cmp(values[9].data.string, values[10].data.string) operator 0));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
	test++;                                                                   \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_STRING, &values[10]);                       \
	stack_push(&calcs, TYPE_STRING, &values[11]);                       \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == (cstr_cmp(values[10].data.string, values[11].data.string) operator 0) ))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is (bool) %d and should be (bool) %d\n", __func__, test, temp.data.boolean, (cstr_cmp(values[10].data.string, values[11].data.string) operator 0));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
END_OF_TEST();                                                                \






#define TEST_OF_BIT_OPERATORS(operation, operator, instruct)                  \
	Type typ;                                                                 \
	Value temp;                                                               \
	int test = 0, errors = 1;                                                 \
	Instruction instruction;                                                  \
	instruction.instruction = instruct;                                       \
	Stack calcs;                                                              \
	stack_init(&calcs, VALUE_STACK);                                          \
	Value values[12];                                                         \
	values[0].data.integer	= 8;                                                    \
	values[1].data.integer	= 100;                                                  \
	values[2].data.integer	= 16;                                                 \
	values[3].data.integer	= 2;                                                    \
	values[4].data.integer	= 8;                                                  \
	values[5].data.integer	= 25;                                                    \
	values[5].data.boolean	= true;                                                 \
	values[6].data.boolean	= false;                                                \
	values[7].data.boolean	= true;                                                 \
	values[8].data.boolean	= false;                                                \
	values[9].data.integer	= 22;                                                   \
	values[10].data.integer	= 1500;                                                \
BEGINE_OF_TEST();                                                             \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_INT, &(values[0]));                       \
	stack_push(&calcs, TYPE_INT, &(values[1]));                       \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_INT && (temp.data.integer == ( values[0].data.integer operator values[1].data.integer )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[0].data.integer operator values[1].data.integer);\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_INT, &(values[3]));                         \
	stack_push(&calcs, TYPE_INT, &(values[4]));                         \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_INT && (temp.data.integer == ( values[3].data.integer operator values[4].data.integer )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, (values[3].data.integer operator values[4].data.integer));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_BOOL, &(values[5]));                      \
	stack_push(&calcs, TYPE_BOOL, &(values[6]));                      \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
		stack_index(&calcs, 0, (int*)&typ, &temp);                            \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[5].data.boolean operator values[6].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is  %d and should be %d\n", __func__, test, temp.data.boolean, (values[5].data.boolean operator values[6].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_BOOL, &(values[5]));                      \
	stack_push(&calcs, TYPE_BOOL, &(values[7]));                      \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_BOOL && (temp.data.boolean == ( values[5].data.boolean operator values[7].data.boolean )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.boolean, (values[5].data.boolean operator values[7].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_INT, &(values[3]));                         \
	stack_push(&calcs, TYPE_INT, &(values[5]));                         \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_INT && (temp.data.integer == ( values[3].data.integer operator values[5].data.integer )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, (values[3].data.integer operator values[5].data.boolean));\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
test++;                                                                       \
printf("\n\n\n%s:[test %d.]------------------------------------------------------------------------------------\n\n", __func__, test);\
	stack_push(&calcs, TYPE_INT, &(values[9]));                               \
	stack_push(&calcs, TYPE_INT, &(values[10]));                              \
	CALL_INTERPRET();                                                         \
	if(calcs.count==1)                                                        \
	{                                                                         \
	stack_index(&calcs, 0, (int*)&typ, &temp);                                                                      \
		if(typ == TYPE_INT && (temp.data.integer == ( values[9].data.integer operator values[10].data.integer )))\
			printf("\n%s:[test %d.] %s was OK.\n", __func__, test, operation);\
		else                                                                  \
		{                                                                     \
			printf("\n%s:[test %d.] ERROR: stack value is %d and should be %d\n", __func__, test, temp.data.integer, values[9].data.integer operator values[10].data.integer);\
			break;                                                            \
		}                                                                     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		printf("\n%s:[test %d.] ERROR: stack counter should be %d and its %u\n", __func__, test, 1, calcs.count);\
		break;                                                                \
	}                                                                         \
	printf("\n");                                                             \
	POP_OUT_LAST();                                                           \
END_OF_TEST();                                                                \








#endif // __TESTS_H__
