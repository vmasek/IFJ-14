/**
 * @file    value.h
 * @brief   Values (header)
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/

#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdbool.h>

#include "cstring.h"

/**
 * Union representing value of any type
 */
typedef struct {
    union {
        int integer;
        double real;
        cstring *string;
        bool boolean;
    } data;
    bool inited;
} Value;


typedef enum
{
    TYPE_OTHER = 0,  //  --
    TYPE_INT,        //  value_int
    TYPE_REAL,       //  value_real      (double)
    TYPE_STRING,     //  value_cstring
    TYPE_BOOL,       //  value bool
} Type;




#endif //__VALUE_H__

