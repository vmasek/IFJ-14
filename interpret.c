/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include "interpret.h"



// Must be also for string, bool etc.?
bool f_less(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) < (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) < (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return ((item->op1->data.string->str) < (item->op2->data.string->str));
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) < (item->op2->data.boolean));
	return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) > (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) > (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return ((item->op1->data.string->str) > (item->op2->data.string->str));
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) > (item->op2->data.boolean));
	return print_error(INCOMPATIBLE_TYPE);
}

bool f_less_equal(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) <= (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) <= (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return ((item->op1->data.string->str) <= (item->op2->data.string->str));
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) <= (item->op2->data.boolean));

	return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater_equal(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) >= (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) >= (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return ((item->op1->data.string->str) >= (item->op2->data.string->str));
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) >= (item->op2->data.boolean));
	return print_error(INCOMPATIBLE_TYPE);
}

bool f_equal(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) == (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) == (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return strcmp(item->op1->data.string->str, item->op2->data.string->str); //is this correct for equal / not equal?
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) == (item->op2->data.boolean));
	return print_error(INCOMPATIBLE_TYPE);
}

bool f_not_equal(T_ITEM *item)
{
	if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
		return ((item->op1->data.integer) != (item->op2->data.integer));
	else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
		return ((item->op1->data.real) != (item->op2->data.real));
	else if ((item->op1->type == Type_CSTRING) && (item->op2->type == Type_CSTRING))
		return strcmp(item->op1->data.string->str, item->op2->data.string->str); // is this correct for equal/notequal?
	else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
		return ((item->op1->data.boolean) != (item->op2->data.boolean));
	return print_error(INCOMPATIBLE_TYPE);
}

/* //Pouzivat iba tieto prvky z enumu
typedef enum
{
	Type_INT,        //  value_int
	Type_DOUBLE,     //  value_double
	Type_CSTRING,    //  value_cstring
	Type_BOOL,       // value bool
} Type;

*/





cstring *read_line(cstring *cstr)
{
	if(!cstr)
		return NULL;

	int ch;
	cstr_clear(cstr);

	while(1)
	{
		ch = getchar();
		if(ch == '\n' || ch == EOF)
			return cstr;
		cstr_append_chr(cstr, ch);
	}
}




//TODO: ERROR CODES
int interpret(T_ITEM *testValue)
{
	switch (testValue->instruction)
	{
	case I_WRITE:																												///OK?
		if (testValue->op1->type == Type_INT)
			printf("%d\n", (testValue->op1->data.integer));
		else if (testValue->op1->type == Type_DOUBLE)
			printf("%f\n", testValue->op1->data.real);
		else if (testValue->op1->type == Type_CSTRING)
			printf("%s\n", testValue->op1->data.string->str);
		else if (testValue->op1->type == Type_BOOL)
			printf("%s", testValue->op1->data.boolean ? "true\n" : "false\n");
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_READLN:																						///OK?
		if (testValue->op1->type == Type_INT)
		{
			debug("I_READLN - integer\n");
			scanf("%d", &(testValue->result->data.integer));
			testValue->result->type = Type_INT;
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			debug("I_READLN - double\n");
			scanf("%lf", &(testValue->result->data.real));
			testValue->result->type = Type_DOUBLE;
		}
		else if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_READLN - cstring\n");
			read_line(testValue->result->data.string=cstr_create_str(""));
			testValue->result->type = Type_CSTRING;
		}
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_ASSIGN:																						///OK?
		if (testValue->op1->type == Type_INT)
		{
			debug("I_ASSIGN - integer\n");
			testValue->result->data.integer = testValue->op1->data.integer;
			testValue->result->type = Type_INT;
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			debug("I_ASSIGN - double\n");
			testValue->result->data.real = testValue->op1->data.real;
			testValue->result->type = Type_DOUBLE;
		}
		else if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_ASSIGN - cstring\n");
			cstr_assign_cstr(testValue->result->data.string, testValue->op1->data.string);
			testValue->result->type = Type_CSTRING;
		}
		else print_error(INCOMPATIBLE_TYPE);
		break;

	//TODO: Strings --- Do we need this for bool? I dont think so.----
	//Is it concat for strings?
	case I_ADD:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			debug("I_ADD for ints\n");
			testValue->result->data.integer = testValue->op1->data.integer + testValue->op2->data.integer;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
		{
			debug("I_ADD for reals\n");
			testValue->result->data.real = testValue->op1->data.real + testValue->op2->data.real;
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			debug("I_ADD for strings\n");
			//cstr_assign_cstr(testValue->op1->data.string);
			//cstr_append_cstr(cstr_assign_cstr(testValue->op1->data.string), testValue->op1->data.string);
			//testValue->result->type = Type_CSTRING;
		}
		//do we really need this condition? Naah i dont this so
		else if (((testValue->op1->type == Type_CSTRING) && (testValue->op2->type != Type_CSTRING)))
		{
			debug("Non compatible type for string concat\n");
			return print_error(INCOMPATIBLE_TYPE);
			break;
		}
		else print_error(INCOMPATIBLE_TYPE);
		break;

	//TODO: Strings, ----Do we need this for string/bool? I dont think so.----
	case I_SUB:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
			testValue->result->data.integer = testValue->op1->data.integer - testValue->op2->data.integer;
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
			testValue->result->data.real = testValue->op1->data.real - testValue->op2->data.real;
		else print_error(INCOMPATIBLE_TYPE);
		break;

	//TODO: Strings,bool, etc
	case I_MULTIPLY:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
			testValue->result->data.integer = testValue->op1->data.integer * testValue->op2->data.integer;
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
			testValue->result->data.real = testValue->op1->data.real * testValue->op2->data.real;
		else print_error(INCOMPATIBLE_TYPE);
		break;

	//TODO: strings,bool, modulo for int?
	case I_DIV:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
		{
			testValue->result->type = Type_DOUBLE;
			testValue->result->data.real = testValue->op1->data.integer / testValue->op2->data.integer;
			testValue->op1->type = Type_DOUBLE;
			testValue->op1->data.real = testValue->result->data.real;
		}
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
			testValue->result->data.real = testValue->op1->data.real / testValue->op2->data.real;
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_LESS:
		if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
			testValue->result->data.boolean = f_less(&*testValue);
		else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
			testValue->result->data.boolean = f_less(&*testValue);
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
			testValue->result->data.boolean = f_less(&*testValue);
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_less(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_GREATER:
		if (testValue->op1->type == Type_INT)
		{
			testValue->result->data.boolean = f_greater(&*testValue);
			debug("I_GREATER - integer\n");
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			testValue->result->data.boolean = f_greater(&*testValue);
			debug("I_GREATER - integer\n");
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			testValue->result->data.boolean = f_greater(&*testValue);
			debug("I_GREATER - string\n");
		}
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_greater(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_LESS_EQUAL:
		if (testValue->op1->type == Type_INT)
		{
			testValue->result->data.boolean = f_less_equal(&*testValue);
			debug("I_LESS_EQUAL - integer\n");
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			testValue->result->data.boolean = f_less_equal(&*testValue);
			debug("I_LESS_EQUAL - double\n");
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			testValue->result->data.boolean = f_less_equal(&*testValue);
			debug("I_GREATER - string\n");
		}
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_less_equal(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_GREATER_EQUAL:
		if (testValue->op1->type == Type_INT)
		{
			testValue->result->data.boolean = f_greater_equal(&*testValue);
			debug("I_GREATER_EQUAL - integer\n");
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			testValue->result->data.boolean = f_greater_equal(&*testValue);
			debug("I_GREATER_EQUAL - double\n");
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
		{
			testValue->result->data.boolean = f_greater_equal(&*testValue);
			debug("I_GREATER - string\n");
		}
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_greater_equal(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_EQUAL:
		if (testValue->op1->type == Type_INT)
		{
			testValue->result->data.boolean = f_equal(&*testValue);
			debug("I_EQUAL - integer\n");
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			testValue->result->data.boolean = f_equal(&*testValue);
			debug("I_EQUAL - double\n");
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
			testValue->result->data.boolean = f_greater_equal(&*testValue);
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_greater_equal(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;

	case I_NOT_EQUAL:
		if (testValue->op1->type == Type_INT)
		{
			testValue->result->data.boolean = f_not_equal(&*testValue);
			debug("I_NOT_EQUAL - integer\n");
		}
		else if (testValue->op1->type == Type_DOUBLE)
		{
			testValue->result->data.boolean = f_not_equal(&*testValue);
			debug("I_NOT_EQUAL - double\n");
		}
		else if ((testValue->op1->type == Type_CSTRING) && (testValue->op2->type == Type_CSTRING))
			testValue->result->data.boolean = f_not_equal(&*testValue);
		else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
			testValue->result->data.boolean = f_not_equal(&*testValue);
		else print_error(INCOMPATIBLE_TYPE);
		break;
	//works fine
	case I_LEN:																																			///OK?
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_LEN - string\n");
			testValue->result->data.integer = length(testValue->op1->data.string);
			testValue->result->type = Type_INT;
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	case I_COPY:																																		//TODO: cstring copy
		if (testValue->op1->type == Type_CSTRING)
		{
			//error: incompatible types when assigning to type ‘cstring’ from type ‘struct cstring *’
			//testValue->result->data.string = copy(&(testValue->op1->data.string), testValue->start, testValue->count);
			debug("I_COPY - string\n");
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	//works fine
	case I_FIND:																																			///OK?
		if (testValue->op1->type == Type_CSTRING)
		{
			debug("I_FIND - string\n");
			testValue->result->data.integer = find(testValue->op1->data.string, testValue->op2->data.string);
			testValue->result->type = Type_INT;
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	// FIX for mastermind Vojtisek
	case I_SORT:
		if (testValue->op1->type == Type_CSTRING)
		{
			// testValue->result->data.string = sort(&(testValue->op1->data.string));
			debug("I_FIND - string\n");
		}
		else
			return print_error(INCOMPATIBLE_TYPE);
		break;

	default:
		return print_error(RUNTIME_OTHER);
		break;
	}
	return 0;
}
