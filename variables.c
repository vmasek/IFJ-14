/**
 * @file    variables.c
 * @brief   Varianles operations
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/

#include "variables.h"

#include "interpreter.h"

int variables_value(Variables *variables, Type *type, Value *value, unsigned int index)
{
	if (!value)
    {
		debug("not Value given.\n");
		return INTERNAL_ERROR;
	}

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



