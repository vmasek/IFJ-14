/**
 * @file    variables.c
 * @brief   Varianles operations
 * @author  Vojtech Mašek (xmasek15), Pavel Tobias (xtobia01)
 ****************************************************************************/

#include "variables.h"

#include "interpreter.h"

int variables_add(Variables *vars, Type type, Value value, unsigned *index)
{
    void *temp;

    if (vars == NULL)
        return INTERNAL_ERROR;

    if ((temp = realloc(vars->values, (vars->count + 1) * sizeof(Value)))
        == NULL)
        return INTERNAL_ERROR;

    vars->values = temp;

    if ((temp = realloc(vars->types, (vars->count + 1) * sizeof(Type)))
        == NULL)
        return INTERNAL_ERROR;

    vars->types = temp;
    vars->types[vars->count] = type;
    vars->values[vars->count] = value;

    if (index != NULL)
        *index = vars->count;

    vars->count++;

    return SUCCESS;
}

int variables_free(Variables *vars)
{
    if (vars == NULL)
        return INTERNAL_ERROR;

    free(vars->values);
    free(vars->types);

    return SUCCESS;
}

int variables_init(Variables *vars)
{
    if (vars == NULL)
        return INTERNAL_ERROR;

    vars->count = 0;
    vars->values = NULL;
    vars->types = NULL;

    return SUCCESS;
}

int variables_occupy(Variables *vars, unsigned count)
{
    void *temp;

    if (vars == NULL)
        return INTERNAL_ERROR;

    if (!count)
        return SUCCESS;

    if ((temp = realloc(vars->values, (vars->count + count) * sizeof(Value)))
        == NULL)
        return INTERNAL_ERROR;

    vars->values = temp;

    if ((temp = realloc(vars->types, (vars->count + count) * sizeof(Type)))
        == NULL)
        return INTERNAL_ERROR;

    vars->types = temp;

    while (count--) {
        vars->types[vars->count] = TYPE_OTHER;
        vars->count++;
    }

    return SUCCESS;
}

int variables_print(Variables *vars)
{
    if (vars == NULL) {
        fprintf(stderr, "NULL-pointer passed to 'vars'!\n");
        return INTERNAL_ERROR;
    }

    if (!vars->count) {
        fprintf(stderr, "No variables to print!\n");
        return SUCCESS;
    }

    for (unsigned i = 0; i < vars->count; i++) {
        switch (vars->types[i]) {
        case TYPE_INT:
            fprintf(stderr, "variable %u: %d (TYPE_INT)\n", i,
                    vars->values[i].integer);
            break;
        case TYPE_REAL:
            fprintf(stderr, "variable %u: %f (TYPE_REAL)\n", i,
                    vars->values[i].real);
            break;
        case TYPE_STRING:
            fprintf(stderr, "variable %u: %s (TYPE_STRING)\n", i,
                    vars->values[i].string->str);
            break;
        case TYPE_BOOL:
            fprintf(stderr, "variable %u: %s (TYPE_BOOL)\n", i,
                    vars->values[i].boolean ? "true" : "false");
            break;
        default:
            fprintf(stderr, "variable %u: ? (UNKNOWN_TYPE)", i);
            break;
        }
    }

    return SUCCESS;
}

int variables_value_read(Variables *variables, Type *type, Value *value, unsigned int index)
{
	if (!value)
    {
		debug("not Value given.\n");
		return INTERNAL_ERROR;
	}

	if (!variables)
    {
		debug("not Variables field given.\n");
		return INTERNAL_ERROR;
	}

	if(type)
		*type = variables->types[index];


	if (variables->types[index] == TYPE_INT)
	{
		debug("integer\n");
		value->integer = variables->values[index].integer;
	}
	else if (variables->types[index] == TYPE_REAL)
	{
		debug("double\n");
		value->real = variables->values[index].real;
	}
	else if (variables->types[index] == TYPE_STRING)
	{
		debug("string\n");
		value->string = variables->values[index].string;
	}
	else if (variables->types[index] == TYPE_BOOL)
	{
		debug("bool\n");
		value->boolean = variables->values[index].boolean;
	}
	else
	{
		debug("bad type\n");
		return INTERNAL_ERROR;
	}

	return SUCCESS;
}

int variables_value_write(Variables *variables, Value *value, unsigned int index)
{
	if (!value)
    {
		debug("not Value given.\n");
		return INTERNAL_ERROR;
	}

	if (!variables)
    {
		debug("not Variables field given.\n");
		return INTERNAL_ERROR;
	}


	variables->values[index] = *value;

	return SUCCESS;
}
