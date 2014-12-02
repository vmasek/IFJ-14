/**
 * @file	common.h
 * @name	Interface with common functions that could be used across modules
 * @author	Albert Uchytil (xuchyt03), Vojtech Mašek (xmasek15)
 *********************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "cstring.h"

/* Macro for gcc to stop nagging about unused parameters */
#define IGNORE_PARAM(id) (void)(id)

/**
 * Debbuging macro
 * Prints out message + info about file, line and the current function.
 * format DEBUG: file.c:line:[function]: Message
 */
#ifdef DEBUG



/*
#define debug(...)                                                            \
    sprintf(msg_buff, __VA_ARGS__);                                           \
    fprintf(stderr, "DEBUG: %s:%d:[%s]:\t\t%s", __FILE__, __LINE__, __func__, msg_buff)\
*/

#define debug(...) debug_printf(__FILE__, __LINE__, __func__, __VA_ARGS__)

#else

#define debug(...)

#endif



/**
 * Value checking macro
 */
#define CHECK_VALUE(val, ret) if (((ret) = (val)) != SUCCESS) return (ret);
#define CATCH_VALUE(val, ret) if (((ret) = (val)) != SUCCESS && (ret) != RETURNING) return (ret)

/**
 * Macro for memory allocation
 */
#define MALLOC_VAR(val, key) if (((val) = (Var_record *)gc_malloc((key), sizeof(Var_record))) == NULL) return INTERNAL_ERROR
#define MALLOC_FUNC(val, key) if (((val) = (Func_record *)gc_malloc((key), sizeof(Func_record))) == NULL) return INTERNAL_ERROR

/**
 * Union representing value of any type
 */
typedef union {
    int integer;
    double real;
    cstring *string;
    bool boolean;
} Value;


typedef enum
{
    TYPE_OTHER = 0,  //  --
    TYPE_INT,        //  value_int
    TYPE_REAL,       //  value_real      (double)
    TYPE_STRING,     //  value_cstring
    TYPE_BOOL,       //  value bool
} Type;


typedef struct
{
    Value data;
    Type type;
} My_value;



/**
 * @brief Function converts char to int.
 * @param c char that's going to be converted
 * @returns integer value between 0 and 9
 *
 * Function converts char to numeric value represented by the char.
 * Returns 0 when the char is not a digit.
 */
extern int char_to_int(char c);

extern void debug_printf(const char* file, const int line, const char* func, const char *fmt, ...);

#endif //__COMMON_H__
