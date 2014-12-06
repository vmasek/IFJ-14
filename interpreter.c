/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include "interpreter.h"


int interpret(Instruction *item, Stack *calcs, Stack *locals, Stack *instructions, Variables *globals)
{
	IGNORE_PARAM(instructions);																						// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Value values[2];
	Type  types[2];
	Value result;


	switch (item->instruction)
	{

	case I_ASSIGN:                                                                                                                                                     ///OK?
		debug("I_ASSIGN\n");

		///                         Stack operations
		if (stack_index(calcs, 0, (int *)&types[0], &result) == INTERNAL_ERROR)
		{
			debug("Invalid read from calcs\n");
			return INTERNAL_ERROR;
		}
		if (stack_pop(calcs) == INTERNAL_ERROR)                                         ///ACHTUNG! If is only one operand then only one pop
		{
			debug("Pop of calcs error\n");
			return INTERNAL_ERROR;
		}

		///                         Instruction operations
		if(item->index < 0)																		/// index indicates locals stack operation
		{
			debug("I_ASSIGN - locals stack\n");
			if (stack_update(locals, (-(item->index+1)), (int)types[0], &result) == INTERNAL_ERROR)
			{
				debug("I_ASSIGN - locals stack error.\n");
				return INTERNAL_ERROR;
			}
		}
		else
		{
			debug("I_ASSIGN - globals field\n");
			if (variables_value_write(globals, &result, item->index) == INTERNAL_ERROR)	/// index indicates global variables field operation
			{
				debug("I_ASSIGN - globals field error.\n");
				return INTERNAL_ERROR;
			}
		}

		break;

	case I_PUSH:                                                                                                                                                     ///OK?

		///                         Value operations
		if(item->index < 0)																		/// index indicates local stack operation
		{
			debug("I_PUSH - locals stack\n");
			//if (stack_index(locals, (-(item->index+1)), (int *)&types[0], &values[0]) == INTERNAL_ERROR) ///
			if (stack_index(locals, (-(item->index+1)), (int *)&types[0], &result) == INTERNAL_ERROR) ///
				return INTERNAL_ERROR;
		}
		else
		{
			debug("I_PUSH - globals field\n");
			//if (variables_value(globals, &types[0], &values[0], item->index) == INTERNAL_ERROR)	/// index indicates global variables operation
			if (variables_value_read(globals, &types[0], &result, item->index) == INTERNAL_ERROR)		/// index indicates global variables field operation
				return INTERNAL_ERROR;
		}
		///ACHTUNG! do not pop or change locals or globals

		///                         Instruction operations
		if (types[0] == TYPE_INT)
		{
			debug("I_PUSH - integer\n");
			//result.integer = values[0].integer;
			stack_push(calcs, TYPE_INT, &result);
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_PUSH - double\n");
			//result.real = values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if (types[0] == TYPE_BOOL)
		{
			debug("I_PUSH - bool\n");
			//result.boolean = values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_PUSH - cstring\n");
			//cstr_assign_cstr(result.string, values[0].string);
			stack_push(calcs, TYPE_STRING, &(result));
		}
		else
		{
			debug("I_PUSH - uninicialized\n");
			return RUNTIME_UNINITIALIZED;
		}
		break;

	case I_ADD:                                                                                                                                                     ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_ADD for ints\n");
			result.integer = values[1].integer + values[0].integer;
			stack_push(calcs, TYPE_INT, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_ADD for reals\n");
			result.real = values[1].real + values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_ADD for real + int\n");
			result.real = values[1].real + values[0].integer;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_ADD for int + real\n");
			result.real = values[1].integer + values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))				/// WARNING: !!!!!!!!!!!! TAKE BIG CARE WHEN RECASTING CSTRING* TO VOID*
		{
			debug("I_ADD for strings\n");
			result.string = cstr_append_cstr(cstr_create_cstr(values[1].string), values[0].string);
			stack_push(calcs, TYPE_STRING, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_SUB:

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_SUB for ints\n");
			result.integer = values[1].integer - values[0].integer;
			stack_push(calcs, TYPE_INT, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_SUB for reals\n");
			result.real = values[1].real - values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_SUB for int - real\n");
			result.real = values[1].integer - values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_SUB for real - int\n");
			result.real = values[1].real - values[0].integer;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_MUL:

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_MULTIPLY - INT\n");
			result.integer = values[1].integer * values[0].integer;
			stack_push(calcs, TYPE_INT, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_MULTIPLY - DOUBLE\n");
			result.real = values[1].real * values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_MULTIPLY for real * int\n");
			result.real = values[1].real * values[0].integer;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_MULTIPLY for int * real\n");
			result.real = values[1].integer * values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_DIV:                                                                                                                                             ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_DIV - INT\n");
			if(values[0].integer == 0)
			{
				debug("Dividing by zero !");
				return RUNTIME_DIV_BY_ZERO;
			}
			result.integer = values[1].integer / values[0].integer;
			stack_push(calcs, TYPE_INT, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_DIV - REAL\n");
			if(values[0].real == 0.0)
			{
				debug("Dividing by zero !");
				return RUNTIME_DIV_BY_ZERO;
			}
			result.real = values[1].real / values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_DIV - REAL / INT\n");
			if(values[0].integer == 0)
			{
				debug("Dividing by zero !");
				return RUNTIME_DIV_BY_ZERO;
			}
			result.real = values[1].real / values[0].integer;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_DIV - INT / REAL\n");
			if(values[0].real == 0.0)
			{
				debug("Dividing by zero !");
				return RUNTIME_DIV_BY_ZERO;
			}
			result.real = values[1].integer / values[0].real;
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_LESS:                                                                                                                                                ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_LESS - INT\n");
			result.boolean = values[1].integer < values[0].integer;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_LESS - DOUBLE\n");
			result.boolean = values[1].real < values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_LESS - BOOL\n");
			result.boolean = values[1].boolean < values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_LESS - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) < 0) ;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_GREATER:                                                                                                                                             ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_GREATER - INT\n");
			result.boolean = values[1].integer > values[0].integer;
			stack_push(calcs, TYPE_BOOL, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_GREATER - DOUBLE\n");
			result.boolean = values[1].real > values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_GREATER - BOOL\n");
			result.boolean = values[1].boolean > values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_GREATER - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) > 0) ;
			debug("bool je %d", cstr_cmp(values[1].string, values[0].string));
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_LESS_EQUAL:                                                                                                                                              ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_LESS_EQUAL - INT\n");
			result.boolean = values[1].integer <= values[0].integer;
			stack_push(calcs, TYPE_BOOL, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_LESS_EQUAL - DOUBLE\n");
			result.boolean = values[1].real <= values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_LESS_EQUAL - BOOL\n");
			result.boolean = values[1].boolean <= values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_LESS_EQUAL - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) <= 0) ;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_GREATER_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_GREATER_EQUAL - INT\n");
			result.boolean = values[1].integer >= values[0].integer;
			stack_push(calcs, TYPE_BOOL, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_GREATER_EQUAL - DOUBLE\n");
			result.boolean = values[1].real >= values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_GREATER_EQUAL - BOOL\n");
			result.boolean = values[1].boolean >= values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_GREATER_EQUAL - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) >= 0) ;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_EQUAL - INT\n");
			result.boolean = values[1].integer == values[0].integer;
			stack_push(calcs, TYPE_BOOL, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_EQUAL - DOUBLE\n");
			result.boolean = values[1].real == values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_EQUAL - BOOL\n");
			result.boolean = values[1].boolean == values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_EQUAL - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) == 0) ;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_NOT_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		STACK_OPERATIONS();

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_NOT_EQUAL - INT\n");
			result.boolean = values[1].integer != values[0].integer;
			stack_push(calcs, TYPE_BOOL, &result);
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_NOT_EQUAL - DOUBLE\n");
			result.boolean = values[1].real != values[0].real;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_BOOL) && (types[1] == TYPE_BOOL))
		{
			debug("I_NOT_EQUAL - BOOL\n");
			result.boolean = values[1].boolean != values[0].boolean;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))
		{
			debug("I_NOT_EQUAL - STRING\n");
			result.boolean = (cstr_cmp(values[1].string, values[0].string) != 0) ;
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_WRITE:

		///                         Stack operations
		if (stack_index(calcs, 0, (int *)&types[0], &result) == INTERNAL_ERROR)
		{
			debug("Invalid read from calcs\n");
			return INTERNAL_ERROR;
		}
		if (stack_pop(calcs) == INTERNAL_ERROR)                                         ///ACHTUNG! If is only one operand then only one pop
		{
			debug("Popping spree calcs error\n");
			return INTERNAL_ERROR;
		}

		if (types[0] == TYPE_INT)
		{
			debug("I_WRITE - INT\n");
			printf("%d\n", result.integer);
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_WRITE - REAL\n");
			printf("%f\n", result.real);
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_WRITE - STRING\n");
			printf("%s\n", result.string->str);
		}
		else if (types[0] == TYPE_BOOL)
		{
			debug("I_WRITE - BOOL\n");
			printf("%s\n", result.boolean ? "true" : "false");
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_READLN:

		if (types[0] == TYPE_INT)
		{
			debug("I_READLN - integer\n");
			//item->result.type = TYPE_INT;
			//scanf("%d", &(item->result.data.integer));
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_READLN - double\n");
			//item->result.type = TYPE_REAL;
			//scanf("%lf", &(item->result.data.real));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_READLN - cstring\n");
			//item->result.type = TYPE_STRING;
			//cstr_read_line(item->result.data.string = cstr_create_str(""));
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}

		///                         Instruction operations
		if(item->index < 0)																		/// index indicates locals stack operation
		{
			debug("I_ASSIGN - locals stack\n");
			if (stack_update(locals, (-(item->index+1)), types[0], &result) == INTERNAL_ERROR)
			{
				debug("I_ASSIGN - locals stack error.\n");
				return INTERNAL_ERROR;
			}
		}
		else
		{
			debug("I_ASSIGN - globals field\n");
			if (variables_value_write(globals, &result, item->index) == INTERNAL_ERROR)	/// index indicates global variables field operation
			{
				debug("I_ASSIGN - globals field error.\n");
				return INTERNAL_ERROR;
			}
		}

		break;

	case I_LEN:

		if (types[0] == TYPE_STRING)
		{
			debug("I_LEN - string\n");
			stack_popping_spree(calcs, 2);
			result.integer = values[0].integer + values[1].integer;
			stack_push(calcs, TYPE_INT, &result);
			//item->result.type = TYPE_INT;
			//item->result.data.integer = length(values[0].string);
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_COPY:                                                                                                                                        ///WRONG !
		if (types[0] == TYPE_STRING)
		{
			debug("I_COPY - string\n");
			//item->result.type = TYPE_STRING;
			//item->result.data.string = copy(values[0].string, values[1].integer, values[1].integer); ///pozor toto bude zmenene na viac operatorov
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_FIND:
		if (types[0] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			result.integer = values[0].integer + values[1].integer; // ?
			stack_push(calcs, TYPE_INT, &result); // ?
			//item->result.type = TYPE_INT;
			//item->result.data.integer = find(values[0].string, values[1].string);
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_SORT:
		if (types[0] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			//item->result.type = TYPE_STRING;
			//item->result.data.string = sort(values[0].string);
		}
		else
		{
			debug("bad type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	default:
		debug("ERROR: Instruction not found.\n");
		return RUNTIME_OTHER;
		break;
	}

	return SUCCESS;
}
