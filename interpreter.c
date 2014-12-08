/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Vojtech Mašek (xmasek15), Tomas Paulus (xpaulu01)
 *******************************************************************/

#include "interpreter.h"


int interpret(Instruction *item, Stack *calcs, Stack *locals, Stack *instructions, Variables *globals)
{

	if(!item || !calcs || !locals || !instructions || !globals)
	{
		debug("Null pointer in given params\n");
		if(!item)
		{
			debug("Item not given.\n");
			return INTERNAL_ERROR;
		}
	}

	Value values[2];
	Type  types[2];
	Value result;


	switch (item->instruction)
	{

	case I_ASSIGN:
		debug("I_ASSIGN\n");
		if(!globals)
		{
			debug("Globals field not given.\n");
			return INTERNAL_ERROR;
		}
		///                         Stack operations
		CALCS_STACK_OPERATIONS_RESULT();

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

	case I_PUSH:
		debug("I_PUSH\n");
		///                         Value operations
		if(item->index < 0)																		/// index indicates local stack operation
		{
			debug("I_PUSH - locals stack\n");
			if (stack_index(locals, (-(item->index+1)), (int *)&types[0], &result) == INTERNAL_ERROR) ///
				return INTERNAL_ERROR;
		}
		else
		{
			debug("I_PUSH - globals field\n");
			if (variables_value_read(globals, &types[0], &result, item->index) == INTERNAL_ERROR)		/// index indicates global variables field operation
				return INTERNAL_ERROR;
		}
		///ACHTUNG! do not pop or change locals or globals

		///                         Instruction operations
		if (types[0] == TYPE_INT)
		{
			debug("I_PUSH - integer\n");
			stack_push(calcs, TYPE_INT, &result);
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_PUSH - double\n");
			stack_push(calcs, TYPE_REAL, &(result));
		}
		else if (types[0] == TYPE_BOOL)
		{
			debug("I_PUSH - bool\n");
			stack_push(calcs, TYPE_BOOL, &(result));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_PUSH - cstring\n");
			stack_push(calcs, TYPE_STRING, &(result));
		}
		else
		{
			debug("I_PUSH - uninicialized\n");
			return RUNTIME_UNINITIALIZED;
		}
		break;

	case I_ADD:

		debug("I_ADD\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS();

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
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_SUB:

		debug("I_SUB\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS();

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
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_MUL:

		debug("I_MUL\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS();

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
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_DIV:

		debug("I_DIV\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS();

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
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_LESS:

		COMPARISON_INSTRUCTION("I_LESS",  < );

	case I_GREATER:

		COMPARISON_INSTRUCTION("I_GREATER",  > );

	case I_LESS_EQUAL:

		COMPARISON_INSTRUCTION("I_LESS_EQUAL", <= );

	case I_GREATER_EQUAL:

		COMPARISON_INSTRUCTION("I_GREATER_EQUAL", >= );

	case I_EQUAL:

		COMPARISON_INSTRUCTION("I_EQUAL", == );

	case I_NOT_EQUAL:

		COMPARISON_INSTRUCTION("I_NOT_EQUAL", != );


	case I_AND:

		LOGICAL_OPERATOR_INSTRUCTION("I_AND", & );

	case I_OR:

		LOGICAL_OPERATOR_INSTRUCTION("II_OR", | );

	case I_XOR:

		LOGICAL_OPERATOR_INSTRUCTION("I_XOR", ^ );

	case I_NOT:

		debug("I_NOT\n");
		CALCS_STACK_OPERATIONS_RESULT();

		if ((types[0] == TYPE_INT))
		{
			debug("I_NOT - INT\n");
			result.integer = ~ result.integer;
			stack_push(calcs, TYPE_INT, &(result));
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_WRITE:

		debug("I_WRITE\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS_RESULT();

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
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_READLN:

		debug("I_READLN\n");
		if (types[0] == TYPE_INT)
		{
			debug("I_READLN - integer\n");
			//scanf("%d", &(item->result.data.integer));
		}
		else if (types[0] == TYPE_REAL)
		{
			debug("I_READLN - double\n");
			//scanf("%lf", &(item->result.data.real));
		}
		else if (types[0] == TYPE_STRING)
		{
			debug("I_READLN - cstring\n");
			//cstr_read_line(item->result.data.string = cstr_create_str(""));
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}

		///                         Instruction operations
		if(item->index < 0)																		/// index indicates locals stack operation
		{
			debug("I_READLN - locals stack\n");
			if (stack_update(locals, (-(item->index+1)), types[0], &result) == INTERNAL_ERROR)
			{
				debug("I_READLN - locals stack error.\n");
				return INTERNAL_ERROR;
			}
		}
		else
		{
			debug("I_READLN - globals field\n");
			if (variables_value_write(globals, &result, item->index) == INTERNAL_ERROR)	/// index indicates global variables field operation
			{
				debug("I_READLN - globals field error.\n");
				return INTERNAL_ERROR;
			}
		}

		break;

	case I_LEN:

		debug("I_LEN\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS_RESULT();

		if (types[0] == TYPE_STRING)
		{
			debug("I_LEN - string\n");

			result.integer = length(result.string);;
			stack_push(calcs, TYPE_INT, &result);
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_COPY:

		debug("I_COPY\n");
	//TODO: dorobit stack operacie

		if (types[0] == TYPE_STRING)
		{
			debug("I_COPY - string\n");
			result.string = copy(result.string, values[0].integer, values[1].integer);
			stack_push(calcs, TYPE_STRING, &result);
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_FIND:

		debug("I_FIND\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS();

		if (types[0] == TYPE_STRING && types[1] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			result.integer = find(values[1].string, values[0].string);
			stack_push(calcs, TYPE_INT, &result);
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_SORT:

		debug("I_SORT\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS_RESULT();

		if (types[0] == TYPE_STRING)
		{
			debug("I_FIND - string\n");
			result.string = sort(result.string);
			stack_push(calcs, TYPE_STRING, &result);
		}
		else
		{
			debug("Invalid type passed to instruction\n");
			return INTERNAL_ERROR;
		}
		break;

	case I_JMP:

		debug("I_JMP\n");
		///                         Stack operations
		CALCS_STACK_OPERATIONS_RESULT();

		///                         Instruction operations
		if (types[0] == TYPE_BOOL)
		{
			if (result.boolean)
			{
				debug("next_instruction\n");
				stack_push(calcs, 0, item->next_instruction);
			}
			else
			{
				debug("alt_instruction\n");
				stack_push(calcs, 0, item->alt_instruction);
			}
		}
		else
		{
			debug("Invalid type passed to instruction\n");
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
