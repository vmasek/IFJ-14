/**
 * @file    builtin.h
 * @author  Tomas Paulus (xpaulu01)
 * @brief   Builtin functions (header file).
 ****************************************************************************/

#ifndef __BUILDIN_H__
#define __BUILDIN_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ial.h"
#include "cstring.h"

/**
 * @brief Counting characters in cstring and returns length of cstring.
 *
 * @param  s cstring String whose characters we want to count.
 * @return   int       Number of characters in string.
 */
int length(cstring *s);

/**
 * @brief Copy characters from cstring in to the cstring from position i and copy
 * as much characters as the value of n is.
 *
 * @param  s cstring    String from which is copied substring.
 * @param  i int        Index at which a substring starts.
 * @param  n int        Number of characters which are copied.
 * @return   cstring    Returns copied substring.
 */
cstring *copy(const cstring *s, int i, int n);

/**
 * @brief Sorting cstring characters by ordinal value.
 *
 * @param  s cstring String which will be sorted.
 * @return   cstring String which was sorted.
 */
int sort(cstring **cstr);

/**
 * @brief Finds first occurence of substring and returns index where substring starts.
 *        If searched substring is empty, returns position 1.
 *
 * @param  s        cstring    String which is sweeped for substring.
 * @param  search   cstring    Substring which is searched for.
 * @return          int        Returns index where ẃas first occurence of substring found.
 *                             If searched substring is empty, returns position 1.
 */
int find(cstring *s, cstring *search);

#endif /* #define __BUILDIN_H__ */
