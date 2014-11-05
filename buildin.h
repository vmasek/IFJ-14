/**
 * @file    buildin.h
 * @name    Buildin functions (header file).
 * @author  Tomas Paulus (xpaulu01)
 * @brief   Implementacia cstrig retazcov pre projekt do predmetu IFJ
 ****************************************************************************/

#ifndef __BUILDIN_H__
#define __BUILDIN_H__

#include "ial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
* @brief Counting characters in string and returns number of characters.
*
* @param  str char* String whose characters we want to count.
* @return     int   Number of characters in string.
*/
int length(char *str);

/**
 * @brief Copy characters from the character in the string at position i and copy
 * as much characters as the value of n is.
 *
 * @param  s char*  String from which is copied substring.
 * @param  i int    Index at which a substring starts.
 * @param  n int    Number of characters which are copied.
 * @return   char*  Returns copied substring.
 */
char *copy(char *s, int i, int n);

/**
 * @brief Sorting string characters by ordinal value.
 *
 * @param  s char* String which will be sorted.
 * @return   char* Sorted string.
 */
char *sort(char *s);

#endif /* #define __BUILDIN_H__ */