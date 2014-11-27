/**
 * @file    buildin.h
 * @name    Implementation of buildin functions.
 * @author  Tomas Paulus (xpaulu01)
 *******************************************************************/

#include "ial.h"
#include "cstring.h"
#include "buildin.h"
#include "errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Counting characters in cstring and returns length of cstring.
 *
 * @param  s cstring String whose characters we want to count.
 * @return   int       Number of characters in string.
 */
int length(cstring *s)
{
    return (s->size);
}

/**
 * @brief Copy characters from cstring in to the cstring from position i and copy
 * as much characters as the value of n is.
 *
 * @param  s cstring    String from which is copied substring.
 * @param  i int        Index at which a substring starts.
 * @param  n int        Number of characters which are copied.
 * @return   cstring    Returns copied substring.
 */
cstring *copy(const cstring *s, int i, int n)
{

    char *string = calloc(n + 1, 1);
    strncpy(string, &s->str[i - 1], n);
    return cstr_create_str(string);
}

/**
 * @brief Finds first occurence of substring and returns index where substring starts.
 *        If searched substring is empty, returns position 1.
 *
 * @param  s        cstring    String which is sweeped for substring.
 * @param  search   cstring    Substring which is searched for.
 * @return          int        Returns index where ẃas first occurence of substring found.
 *                             If searched substring is empty, returns position 1.
 *                             If string where to search is empty returns 0.
 */
int find(cstring *s, cstring *search)
{
	if (strcmp(s->str, "") == 0)
        return 0;
    else if (strcmp(search->str, "") == 0)
        return 1;
    else
        return (kmp_substr(s->str, search->str) + 1);
}

/**
 * @brief Sorting cstring characters by ordinal value.
 *
 * @param  s cstring String which will be sorted.
 * @return   cstring String which was sorted.
 */
cstring *sort(cstring *s)
{
    ms_sort(s->str);
    return s;
}
