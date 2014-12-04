/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include "interpreter.h"




int interpret(Instruction *item, Stack *calcs, Stack *locals, Stack *instructions)
{
	///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	IGNORE_PARAM(locals);
	IGNORE_PARAM(instructions);

	Value values[2];
	Type  types[2];
	Value *result = calloc(sizeof(Value), 1);

	//void *tmp_ptr= NULL;


	switch (item->instruction)
	{

	case I_WRITE:
		if (types[0] == TYPE_INT)
			printf("%d\n", (values[0].integer));
		else if (types[0] == TYPE_REAL)
			printf("%f\n", values[0].real);
		else if (types[0] == TYPE_STRING)
			printf("%s\n", values[0].string->str);
		else if (types[0] == TYPE_BOOL)
			printf("%s\n", values[0].boolean ? "true" : "false");
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_READLN:
		if (types[0] == TYPE_INT)
		{
			debug("I_READLN - integer\n");
			//item->result->type = TYPE_INT;
			//scanf("%d", &(item->result->data.integer));
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_READLN - double\n");
			//item->result->type = TYPE_REAL;
			//scanf("%lf", &(item->result->data.real));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_READLN - cstring\n");
			//item->result->type = TYPE_STRING;
			//cstr_read_line(item->result->data.string = cstr_create_str(""));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_ASSIGN:                                                                                                                                                     ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;
		if (stack_pop(calcs) == INTERNAL_ERROR)                                         ///ACHTUNG! If is only one operand then only one pop
			return INTERNAL_ERROR;

		///                         Instruction operations
		if (types[0] == TYPE_INT)
		{
			debug("I_ASSIGN - integer\n");
			result->integer = values[0].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_ASSIGN - double\n");
			result->real = values[0].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if (types[0] == TYPE_BOOL)
		{
			debug("I_ASSIGN - bool\n");
			result->boolean = values[0].boolean;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->boolean));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_ASSIGN - cstring\n");
			cstr_assign_cstr(result->string, values[0].string);
			stack_push(calcs, TYPE_STRING, (void *)(result->string));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_ADD:                                                                                                                                                     ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_ADD for ints\n");
			result->integer = values[0].integer + values[1].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_ADD for reals\n");
			result->real = values[0].real + values[1].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_ADD for int + real\n");
			result->real = values[0].integer + values[1].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_ADD for real + int\n");
			result->real = values[0].real + values[1].integer;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING)) //@Vojto: you know what to do.
		{
			debug("I_ADD for strings\n");
			//item->result->type = TYPE_STRING;
			//cstr_append_cstr(cstr_assign_cstr(item->result->data.string, values[0].string), values[1].string);
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_SUB:

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_SUB for ints\n");
			result->integer = values[1].integer - values[0].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_SUB for reals\n");
			result->real = values[1].real - values[0].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_SUB for int - real\n");
			result->real = values[1].real - values[0].integer; //do not work, fuck this shit
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_SUB for real - int\n");
			result->real = values[0].integer - values[1].real; //do not work, fuck this shit
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	///OK? ///Maybe later: cstrings or bool
	/// Tom: I dont think that cstrings or booleans can be multiplied.
	case I_MUL:

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_MULTIPLY - INT\n");
			result->integer = values[0].integer * values[1].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_MULTIPLY - DOUBLE\n");
			result->real = values[0].real * values[1].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
		{
			debug("I_MULTIPLY for int + real\n");
			result->real = values[0].integer * values[1].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
		{
			debug("I_MULTIPLY for real + int\n");
			result->real = values[0].real * values[1].integer;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_DIV:                                                                                                                                             ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_DIV - INT\n");
			result->integer = values[1].integer / values[0].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_DIV - DOUBLE\n");
			result->real = values[1].real / values[0].real;
			stack_push(calcs, TYPE_REAL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_LESS:                                                                                                                                                ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_LESS - INT\n");
			result->boolean = values[1].integer < values[0].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_LESS - DOUBLE\n");
			result->boolean = values[1].real < values[0].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_GREATER:                                                                                                                                             ///OK?
		
		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_GREATER - INT\n");
			result->boolean = values[0].integer > values[1].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_GREATER - DOUBLE\n");
			result->boolean = values[0].real > values[1].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_LESS_EQUAL:                                                                                                                                              ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_LESS_EQUAL - INT\n");
			result->boolean = values[0].integer <= values[1].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_LESS_EQUAL - DOUBLE\n");
			result->boolean = values[0].real <= values[1].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_GREATER_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_GREATER_EQUAL - INT\n");
			result->boolean = values[0].integer >= values[1].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_GREATER_EQUAL - DOUBLE\n");
			result->boolean = values[0].real >= values[1].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_EQUAL - INT\n");
			result->boolean = values[0].integer == values[1].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_EQUAL - DOUBLE\n");
			result->boolean = values[0].real == values[1].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING)) //@Vojto: you know what to do.
		{
			debug("I_EQUAL - CSTRING\n");
			//item->result->type = TYPE_BOOL;
			//item->result->data.boolean = ((cstr_cmp(values[0].string, values[1].string) == 0));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_NOT_EQUAL:                                                                                                                                               ///OK?

		///                         Stack operations
		if (stack_index_value(calcs, 0, (int *)&types[0], &values[0]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_index_value(calcs, 1, (int *)&types[1], &values[1]) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		if (stack_popping_spree(calcs, 2) == INTERNAL_ERROR)
			return INTERNAL_ERROR;

		///                         Instruction operations
		if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
		{
			debug("I_NOT_EQUAL - INT\n");
			result->boolean = values[0].integer != values[1].integer;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->integer));
		}
		else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
		{
			debug("I_NOT_EQUAL - DOUBLE\n");
			result->boolean = values[0].real != values[1].real;
			stack_push(calcs, TYPE_BOOL, (void *) & (result->real));
		}
		else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING)) //@Vojto: you know what to do.
		{
			debug("I_NOT_EQUAL - CSTRING\n");
			//item->result->type = TYPE_BOOL;
			//item->result->data.boolean = ((cstr_cmp(values[0].string, values[1].string) != 0));
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	//works fine
	case I_LEN:

		if (types[0] == TYPE_STRING)
		{
			debug("I_LEN - string\n");
			stack_popping_spree(calcs, 2);
			result->integer = values[0].integer + values[1].integer;
			stack_push(calcs, TYPE_INT, (void *) & (result->integer));
			//item->result->type = TYPE_INT;
			//item->result->data.integer = length(values[0].string);
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_COPY:                                                                                                                                        ///WRONG !
		if (types[0] == TYPE_STRING)
		{
			debug("I_COPY - string\n");
			//item->result->type = TYPE_STRING;
			//item->result->data.string = copy(values[0].string, values[1].integer, values[1].integer); ///pozor toto bude zmenene na viac operatorov
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_FIND:
		if (types[0] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			result->integer = values[0].integer + values[1].integer; // ?
			stack_push(calcs, TYPE_INT, (void *) & (result->integer)); // ?
			//item->result->type = TYPE_INT;
			//item->result->data.integer = find(values[0].string, values[1].string);
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	case I_SORT:
		if (types[0] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			//item->result->type = TYPE_STRING;
			//item->result->data.string = sort(values[0].string);
		}
		else
			return INCOMPATIBLE_TYPE;
		break;

	default:
		debug("ERROR: Instruction not found.\n");
		return RUNTIME_OTHER;
		break;
	}

	return SUCCESS;
}