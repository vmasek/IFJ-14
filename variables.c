/**
 * @file    variables.c
 * @brief   Varianles operations
 * @author  Vojtech Mašek (xmasek15), Pavel Tobias (xtobia01)
 ****************************************************************************/

#include <stdlib.h>

#include "debug.h"
#include "variables.h"
#include "errors.h"

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

int variables_add_empty(Variables *vars, Type type)
{
    return variables_add(vars, type, (Value) {.inited = false}, NULL);
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
    const char *type_strs[] = {"TYPE_OTHER", "TYPE_INT", "TYPE_REAL",
                               "TYPE_STRING", "TYPE_BOOL"};

    if (vars == NULL) {
        fprintf(stderr, "NULL-pointer passed to 'vars'!\n");
        return INTERNAL_ERROR;
    }

    if (!vars->count) {
        fprintf(stderr, "No variables to print!\n");
        return SUCCESS;
    }

    for (unsigned i = 0; i < vars->count; i++) {
        if (!vars->values[i].inited) {
            fprintf(stderr, "variable %u: <uninitialized> (%s)\n", i,
                    type_strs[vars->types[i]]);
            continue;
        }

        switch (vars->types[i]) {
        case TYPE_INT:
            fprintf(stderr, "variable %u: %d (TYPE_INT)\n", i,
                    vars->values[i].data.integer);
            break;
        case TYPE_REAL:
            fprintf(stderr, "variable %u: %f (TYPE_REAL)\n", i,
                    vars->values[i].data.real);
            break;
        case TYPE_STRING:
            fprintf(stderr, "variable %u: %s (TYPE_STRING)\n", i,
                    vars->values[i].data.string->str);
            break;
        case TYPE_BOOL:
            fprintf(stderr, "variable %u: %s (TYPE_BOOL)\n", i,
                    vars->values[i].data.boolean ? "true" : "false");
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

	if(index >= variables->count)
	{
		debug("Variables index %u is out of range\n", index);
		return INTERNAL_ERROR;
	}

	if(type)
		*type = variables->types[index];

	*value = variables->values[index];

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

	if(index >= variables->count)
	{
		debug("Variables index %u is out of range\n", index);
		return INTERNAL_ERROR;
	}

	variables->values[index] = *value;

	return SUCCESS;
}
