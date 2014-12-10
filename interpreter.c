/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Vojtech Mašek (xmasek15), Tomas Paulus (xpaulu01)
 *******************************************************************/

#include "interpreter.h"


int interpret(Instruction *item, Variables *globals)
{
	if(!item || !globals)
	{
		debug("Null pointer in given params\n");
		if(!item)
		{
			debug("Item not given.\n");
			return INTERNAL_ERROR;
		}
	}

	int error;
	Stack calcs;
	Stack locals;
	Stack instructions;
	Value values[2];
	Type  types[2];
	Value result = {.inited = true};

	stack_init(&calcs, VALUE_STACK);
	stack_init(&locals, VALUE_STACK);
	stack_init(&instructions, INSTR_STACK);

	while(item)
	{
		switch (item->instruction)
		{

		case I_ASSIGN:
			debug("I_ASSIGN\n");
			if(!globals)
			{
				debug("Globals field not given.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			///                         Instruction operations
			if(item->index < 0)																		/// index indicates locals stack operation
			{
				debug("I_ASSIGN - locals stack\n");
				if (stack_update(&locals, (-(item->index+1)), (int)types[0], &result) == INTERNAL_ERROR)
				{
					debug("I_ASSIGN - locals stack error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}
			else
			{
				debug("I_ASSIGN - globals field\n");
				if (variables_value_write(globals, &result, item->index) == INTERNAL_ERROR)	/// index indicates global variables field operation
				{
					debug("I_ASSIGN - globals field error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}

			item = item->next_instruction;
			break;

		case I_PUSH:
			debug("I_PUSH\n");
			///                         Value operations
			if(item->index < 0)																		/// index indicates local stack operation
			{
				debug("I_PUSH - locals stack\n");
				if (stack_index(&locals, (-(item->index+1)), (int *)&types[0], &result) == INTERNAL_ERROR) ///
				{
					debug("I_PUSH - locals stack_index error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}
			else
			{
				debug("I_PUSH - globals field\n");
				if (variables_value_read(globals, &types[0], &result, item->index) == INTERNAL_ERROR)		/// index indicates global variables field operation
				{
					debug("I_PUSH - globals variables_value_read error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}
			///ACHTUNG! do not pop or change locals or globals
			if (!result.inited)
			{
				debug("I_PUSH - uninitialized\n");
				error = RUNTIME_UNINITIALIZED;
				goto fail;
			}
			///                         Instruction operations
			stack_push(&calcs, types[0], &result);

			item = item->next_instruction;
			break;

		case I_PASS:
			debug("I_PASS\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			///                         Instruction operations
			debug("I_PASS - pushing to locals stack\n");
			if (stack_push(&locals, (int)types[0], &result) == INTERNAL_ERROR)
			{
				debug("I_PASS - locals stack push error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}


			item = item->next_instruction;
			break;

		case I_PREP:
			debug("I_PREP - pushing to locals stack\n");
			if (stack_push(&locals, item->index, NULL) == INTERNAL_ERROR)
			{
				debug("I_PREP - locals stack push error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}


			item = item->next_instruction;
			break;

		case I_CALL:
			debug("I_CALL\n");

			if (stack_push(&instructions, 0, item->next_instruction) == INTERNAL_ERROR)
			{
				debug("I_CALL - instructions stack push error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->alt_instruction;
			break;

		case I_HALT:
			debug("I_HALT\n");

			if (stack_index(&locals, 0, (int *)&types[0], &result) == INTERNAL_ERROR) ///get the top of locals (it is return code)
			{
				debug("I_HALT - locals stack_index (top) error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			if (stack_push(&locals, item->index, NULL) == INTERNAL_ERROR)
			{
				debug("I_HALT - calcs stack push error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			if(stack_popping_spree(&locals, item->index)==INTERNAL_ERROR) ///number of requied pop is in index
			{
				debug("I_HALT - Popping spree locals error.\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			if(&instructions.top == NULL)
			{
				debug("I_HALT - Successfull end of interpretation.");
				item = NULL;
				continue;
			}
			else
			{
				debug("I_HALT - continue.");
			}


			item = item->next_instruction;
			break;

		case I_ADD:
			debug("I_ADD\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS();

			///                         Instruction operations
			if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
			{
				debug("I_ADD for ints\n");
				result.data.integer = values[1].data.integer + values[0].data.integer;
				stack_push(&calcs, TYPE_INT, &result);
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
			{
				debug("I_ADD for reals\n");
				result.data.real = values[1].data.real + values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
			{
				debug("I_ADD for real + int\n");
				result.data.real = values[1].data.real + values[0].data.integer;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
			{
				debug("I_ADD for int + real\n");
				result.data.real = values[1].data.integer + values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_STRING) && (types[1] == TYPE_STRING))				/// WARNING: !!!!!!!!!!!! TAKE BIG CARE WHEN RECASTING CSTRING* TO VOID*
			{
				debug("I_ADD for strings\n");
				result.data.string = cstr_append_cstr(cstr_create_cstr(values[1].data.string), values[0].data.string);
				stack_push(&calcs, TYPE_STRING, &(result));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_SUB:
			debug("I_SUB\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS();

			///                         Instruction operations
			if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
			{
				debug("I_SUB for ints\n");
				result.data.integer = values[1].data.integer - values[0].data.integer;
				stack_push(&calcs, TYPE_INT, &result);
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
			{
				debug("I_SUB for reals\n");
				result.data.real = values[1].data.real - values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
			{
				debug("I_SUB for int - real\n");
				result.data.real = values[1].data.integer - values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
			{
				debug("I_SUB for real - int\n");
				result.data.real = values[1].data.real - values[0].data.integer;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_MUL:
			debug("I_MUL\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS();

			///                         Instruction operations
			if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
			{
				debug("I_MULTIPLY - INT\n");
				result.data.integer = values[1].data.integer * values[0].data.integer;
				stack_push(&calcs, TYPE_INT, &result);
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
			{
				debug("I_MULTIPLY - DOUBLE\n");
				result.data.real = values[1].data.real * values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
			{
				debug("I_MULTIPLY for real * int\n");
				result.data.real = values[1].data.real * values[0].data.integer;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
			{
				debug("I_MULTIPLY for int * real\n");
				result.data.real = values[1].data.integer * values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_DIV:
			debug("I_DIV\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS();

			///                         Instruction operations
			if ((types[0] == TYPE_INT) && (types[1] == TYPE_INT))
			{
				debug("I_DIV - INT\n");
				if(values[0].data.integer == 0)
				{
					debug("Dividing by zero !");
					error = RUNTIME_DIV_BY_ZERO;
					goto fail;
				}
				result.data.integer = values[1].data.integer / values[0].data.integer;
				stack_push(&calcs, TYPE_INT, &result);
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_REAL))
			{
				debug("I_DIV - REAL\n");
				if(values[0].data.real == 0.0)
				{
					debug("Dividing by zero !");
					error = RUNTIME_DIV_BY_ZERO;
					goto fail;
				}
				result.data.real = values[1].data.real / values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_INT) && (types[1] == TYPE_REAL))
			{
				debug("I_DIV - REAL / INT\n");
				if(values[0].data.integer == 0)
				{
					debug("Dividing by zero !");
					error = RUNTIME_DIV_BY_ZERO;
					goto fail;
				}
				result.data.real = values[1].data.real / values[0].data.integer;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else if ((types[0] == TYPE_REAL) && (types[1] == TYPE_INT))
			{
				debug("I_DIV - INT / REAL\n");
				if(values[0].data.real == 0.0)
				{
					debug("Dividing by zero !");
					error = RUNTIME_DIV_BY_ZERO;
					goto fail;
				}
				result.data.real = values[1].data.integer / values[0].data.real;
				stack_push(&calcs, TYPE_REAL, &(result));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
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
				result.data.integer = ~ result.data.integer;
				stack_push(&calcs, TYPE_INT, &(result));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_WRITE:
			debug("I_WRITE\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			if (types[0] == TYPE_INT)
			{
				debug("I_WRITE - INT\n");
				printf("%d\n", result.data.integer);
			}
			else if (types[0] == TYPE_REAL)
			{
				debug("I_WRITE - REAL\n");
				printf("%f\n", result.data.real);
			}
			else if (types[0] == TYPE_STRING)
			{
				debug("I_WRITE - STRING\n");
				printf("%s\n", result.data.string->str);
			}
			else if (types[0] == TYPE_BOOL)
			{
				debug("I_WRITE - BOOL\n");
				printf("%s\n", result.data.boolean ? "true" : "false");
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_READLN:
			debug("I_READLN\n");
			if (types[0] == TYPE_INT)
			{
				debug("I_READLN - integer\n");
				//scanf("%d", &(item->result.data.data.integer));
			}
			else if (types[0] == TYPE_REAL)
			{
				debug("I_READLN - double\n");
				//scanf("%lf", &(item->result.data.data.real));
			}
			else if (types[0] == TYPE_STRING)
			{
				debug("I_READLN - cstring\n");
				//cstr_read_line(item->result.data.data.string = cstr_create_str(""));
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			///                         Instruction operations
			if(item->index < 0)																		/// index indicates locals stack operation
			{
				debug("I_READLN - locals stack\n");
				if (stack_update(&locals, (-(item->index+1)), types[0], &result) == INTERNAL_ERROR)
				{
					debug("I_READLN - locals stack error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}
			else
			{
				debug("I_READLN - globals field\n");
				if (variables_value_write(globals, &result, item->index) == INTERNAL_ERROR)	/// index indicates global variables field operation
				{
					debug("I_READLN - globals field error.\n");
					error = INTERNAL_ERROR;
					goto fail;
				}
			}


			item = item->next_instruction;
			break;

		case I_LEN:
			debug("I_LEN\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			if (types[0] == TYPE_STRING)
			{
				debug("I_LEN - string\n");

				result.data.integer = length(result.data.string);
				stack_push(&calcs, TYPE_INT, &result);
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_COPY:
			debug("I_COPY\n");
		//TODO: dorobit stack operacie

			if (types[0] == TYPE_STRING)
			{
				debug("I_COPY - string\n");
				result.data.string = copy(result.data.string, values[0].data.integer, values[1].data.integer);
				stack_push(&calcs, TYPE_STRING, &result);
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_FIND:
			debug("I_FIND\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS();

			if (types[0] == TYPE_STRING && types[1] == TYPE_STRING)
			{
				debug("I_FIND - string\n");
				result.data.integer = find(values[1].data.string, values[0].data.string);
				stack_push(&calcs, TYPE_INT, &result);
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_SORT:
			debug("I_SORT\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			if (types[0] == TYPE_STRING)
			{
				debug("I_FIND - string\n");
				result.data.string = sort(result.data.string);
				stack_push(&calcs, TYPE_STRING, &result);
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			item = item->next_instruction;
			break;

		case I_JMP:
			debug("I_JMP\n");
			///                         Stack operations
			CALCS_STACK_OPERATIONS_RESULT();

			///                         Instruction operations
			if (types[0] == TYPE_BOOL)
			{
				if (result.data.boolean)
				{
					debug("I_JMP - next_instruction\n");
					item = item->next_instruction;
				}
				else
				{
					debug("I_JMP - alt_instruction\n");
					item = item->alt_instruction;
				}
			}
			else
			{
				debug("Invalid type passed to instruction\n");
				error = INTERNAL_ERROR;
				goto fail;
			}

			break;

		case I_NOP:
			debug("I_NOP\n");
			debug("I_NOP - sets next instruction.\n");
			item = item->next_instruction;
			break;

		default:
			debug("ERROR: Instruction not found.\n");
			error = RUNTIME_OTHER;
			goto fail;

		}


	}

	error = SUCCESS;

fail:
	stack_free(&calcs);
	stack_free(&locals);
	stack_free(&instructions);

	return error;
}
