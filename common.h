/**
 * @file	common.h
 * @name	Interface with common functions that could be used across modules
 * @author	Albert Uchytil (xuchyt03), Vojtech Mašek (xmasek15)
 *********************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include "cstring.h"

/**
 * Debbugin macro
 * Prints out message + info about file and line.
 * format DEBUG: file.c:line:[stack_push]:
 */
#ifdef DEBUG
#define debug(Message) fprintf(stderr, "DEBUG: %s:%d:[%s]:\t\t" Message "\n", __FILE__, __LINE__, __func__)
#else
#define debug(Message)

#endif

/**
 * Value checking macro
 */
#define CHECK_VALUE(val, ret) if (((ret) = (val)) != SUCCESS) return (ret);

/**
 * Union representing value of any type
 */
typedef union {
    int integer;
    double real;
    cstring string;
    bool boolean;
} Value;

/**
 * @brief Function converts char to int.
 * @param c char that's going to be converted
 * @returns integer value between 0 and 9
 *
 * Function converts char to numeric value represented by the char.
 * Returns 0 when the char is not a digit.
 */
int char_to_int(char c);

#endif //__COMMON_H__
