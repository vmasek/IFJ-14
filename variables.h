/**
 * @file    variables.h
 * @brief   Variables operations (header)
 * @author  Vojtech Mašek (xmasek15)
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


int variables_value(Variables*, Type*, Value*, unsigned int);




#endif //__VARIABLES_H__
