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

inline void debug_printf(const char* file, const int line, const char* func, const char *fmt, ...)
{
	va_list arg;
	static char format[1000]={0};
	sprintf(format, "DEBUG: %s:%d:[%s]:\t\t", file, line, func);
	va_start(arg, fmt);
	strcat(format, fmt);
	vfprintf(stderr, format, arg);
	va_end(arg);
}
//__FILE__, __LINE__, __func__
