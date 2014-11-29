/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include "interpret.h"


//refactor Type_DOUBLE to Type_REAL

int interpret(T_ITEM *testValue)
{
	switch (testValue->instruction)
	{
	case I_WRITE:                                                                                                                                                   ///OK?
		if (testValue->op1->type == Type_INT)
			printf("%d\n", (testValue->op1->data.integer));
		else if (testValue->op1->type == Type_DOUBLE)
			printf("%f\n", testValue->op1->data.real);
		else if (testValue->op1->type == Type_CSTRING)
			printf("%s\n", testValue->op1->data.string->str);
		else if (testValue->op1->type == Type_BOOL)
			printf("%s\n", testValue->op1->data.boolean ? "true" : "false");
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_READLN:                                                                                                                                                  ///OK?
		if (testValue->op1->type == Type_INT)
		{
			debug("I_READLN - integer\n");
			testValue->result->type = Type_INT;
			scanf("%d", &(testValue->result->data.integer));
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			debug("I_READLN - double\n");
			testValue->result->type = Type_DOUBLE;
			scanf("%lf", &(testValue->result->data.real));
		}
		else if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_READLN - cstring\n");
			testValue->result->type = Type_CSTRING;
			cstr_read_line(testValue->result->data.string = cstr_create_str(""));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_ASSIGN:                                                                                                                                                  ///OK?
		if (testValue->op1->type == Type_INT)
		{
			debug("I_ASSIGN - integer\n");
			testValue->result->type = Type_INT;
			testValue->result->data.integer = testValue->op1->data.integer;
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			debug("I_ASSIGN - double\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real;
		}
		else if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_ASSIGN - cstring\n");
			testValue->result->type = Type_CSTRING;
			cstr_assign_cstr(testValue->result->data.string, testValue->op1->data.string);
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_ADD:                                                                                                                                                     ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_ADD for ints\n");
			testValue->result->type = Type_INT;
			testValue->result->data.integer = testValue->op1->data.integer + testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_ADD for reals\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real + testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_Double))
		{
			debug("I_ADD for int + real\n");
			testValue->result->type == Type_DOUBLE; // for different type of operands, we need to change result type
			testValue->result->data.real = testValue->op1->data.integer + testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_INT))
		{
			debug("I_ADD for real + int\n");
			testValue->result->type == Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real + testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			debug("I_ADD for strings\n");
			testValue->result->type = Type_CSTRING;
			cstr_append_cstr(cstr_assign_cstr(testValue->result->data.string, testValue->op1->data.string), testValue->op2->data.string);
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_SUB:                                                                                                                                             ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_SUB for ints\n");
			testValue->result->type = Type_INT;
			testValue->result->data.integer = testValue->op1->data.integer - testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_SUB for reals\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real - testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_Double))
		{
			debug("I_SUB for int + real\n");
			testValue->result->type == Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.integer - testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_INT))
		{
			debug("I_SUB for real + int\n");
			testValue->result->type == Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real - testValue->op2->data.integer;
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	///OK? ///Maybe later: cstrings or bool
	/// Tom: I dont think that cstrings or booleans can be multiplied.
	case I_MULTIPLY:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_DIV - INT\n");
			testValue->result->type = Type_INT;
			testValue->result->data.real = testValue->op1->data.integer * testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_DIV - DOUBLE\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real * testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_Double))
		{
			debug("I_MULTIPLY for int + real\n");
			testValue->result->type == Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.integer * testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_INT))
		{
			debug("I_MULTIPLY for real + int\n");
			testValue->result->type == Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real * testValue->op2->data.integer;
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_DIV:                                                                                                                                             ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_DIV - INT\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.integer / testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_DIV - DOUBLE\n");
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.real / testValue->op2->data.real;
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_LESS:                                                                                                                                                ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_LESS - INT\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.integer) < (testValue->op2->data.integer));
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_LESS - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) < (testValue->op2->data.real));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_GREATER:                                                                                                                                             ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_GREATER - INT\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.integer) > (testValue->op2->data.integer));
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_GREATER - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) > (testValue->op2->data.real));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_LESS_EQUAL:                                                                                                                                              ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_LESS_EQUAL - INT\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.integer) <= (testValue->op2->data.integer));
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_LESS_EQUAL - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) <= (testValue->op2->data.real));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_GREATER_EQUAL:                                                                                                                                               ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_GREATER_EQUAL - INT\n");
			testValue->result->data.boolean = ((testValue->op1->data.integer) >= (testValue->op2->data.integer));
			testValue->result->type = Type_BOOL;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_GREATER_EQUAL - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) >= (testValue->op2->data.real));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_EQUAL:                                                                                                                                               ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_EQUAL - INT\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.integer) == (testValue->op2->data.integer));
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_EQUAL - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) == (testValue->op2->data.real));
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			debug("I_EQUAL - CSTRING\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((cstr_cmp(testValue->op1->data.string, testValue->op2->data.string) == 0));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	case I_NOT_EQUAL:                                                                                                                                               ///OK?
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_NOT_EQUAL - INT\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.integer) != (testValue->op2->data.integer));
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_NOT_EQUAL - DOUBLE\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((testValue->op1->data.real) != (testValue->op2->data.real));
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			debug("I_NOT_EQUAL - CSTRING\n");
			testValue->result->type = Type_BOOL;
			testValue->result->data.boolean = ((cstr_cmp(testValue->op1->data.string, testValue->op2->data.string) != 0));
		}
		else
			print_error(INCOMPATIBLE_TYPE);
		break;

	//works fine
	case I_LEN:                                                                                                                                                 ///OK?
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_LEN - string\n");
			testValue->result->type = Type_INT;
			testValue->result->data.integer = length(testValue->op1->data.string);
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	case I_COPY:                                                                                                                                        ///WRONG !
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_COPY - string\n");
			testValue->result->type = Type_CSTRING;
			testValue->result->data.string = copy(testValue->op1->data.string, testValue->op2->data.integer, testValue->op2->data.integer); ///pozor toto bude zmenene na viac operatorov
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	case I_FIND:                                                                                                                                            ///OK?
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_FIND - string\n");
			testValue->result->type = Type_INT;
			testValue->result->data.integer = find(testValue->op1->data.string, testValue->op2->data.string);
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	case I_SORT:
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_FIND - string\n");
			testValue->result->type = Type_CSTRING;
			testValue->result->data.string = sort(testValue->op1->data.string);
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	default:
		debug("ERROR: Instruction not found.\n");
		return print_error(RUNTIME_OTHER);
		break;
	}

	return 0;
}
