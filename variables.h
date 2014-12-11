/**
 * @file    variables.h
 * @brief   Variables operations (header)
 * @author  Vojtech Mašek (xmasek15), Pavel Tobias (xtobia01)
 ****************************************************************************/

#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#include "common.h"


/// Struct of storing (global) variables
typedef struct
{
    unsigned int count;     ///count of all items
    Value *values;            ///field of Values
    Type *types;             ///field of Types paralel to Values
} Variables;


int variables_add(Variables *vars, Type type, Value value, unsigned *index);
int variables_add_empty(Variables *vars, Type type);
int variables_free(Variables *vars);
int variables_init(Variables *vars);
int variables_occupy(Variables *vars, unsigned count);
int variables_print(Variables *vars);
int variables_value_read(Variables*, Type*, Value*, unsigned int);
int variables_value_write(Variables*, Value*, unsigned int);


#endif //__VARIABLES_H__
