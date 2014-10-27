/**
 * @file	common.c
 * @name	Common functions that could be used across modules
 * @author	Albert Uchytil (xuchyt03)
 ****************************************************************************/

#include "common.h"

/**
 * @brief Function converts char to int.
 * @param c char that's going to be converted
 * @returns integer value between 0 and 9
 *
 * Function converts char to numeric value represented by the char.
 * Returns 0 when the char is not a digit.
 */
inline int char_to_int(char c)
{
    return ('0' <= c && c <= '9') ? (int)(c - '0') : 0;
}
