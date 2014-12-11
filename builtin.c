/**
 * @file    builtin.h
 * @brief   Implementation of buildin functions.
 * @author  Tomas Paulus (xpaulu01)
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buildin.h"
#include "cstring.h"
#include "debug.h"
#include "errors.h"
#include "ial.h"


/**
 * @brief Counting characters in cstring and returns length of cstring.
 *
 * @param  s cstring String whose characters we want to count.
 * @return   int       Number of characters in string. (-1) if error.
 *
 */
int length(cstring *s)
{
	if(!s)
	{
		debug("cstring not given\n");
		return -1;
	}

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
	if(!s)
	{
		debug("cstring not given\n");
		return NULL;
	}

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
 *                             IF error -1.
 */
int find(cstring *s, cstring *search)
{
	if(!s || !search)
	{
		debug("cstrings not given\n");
		return -1;
	}

	if (s->size == 0)
	{
		debug("can not search in empty string\n");
		return 0;
	}
    else if (search->size == 0)
	{
		debug("searched is empty\n");
		return 1;
	}
    else
	{
		debug("calling kmp_substr\n");
		return (kmp_substr(s->str, search->str) + 1);
	}
}


/**
 * @brief Sorting cstring characters by ordinal value.
 *
 * @param  s cstring String which will be sorted.
 * @return   cstring String which was sorted.
 */
cstring *sort(cstring *s)
{
	if(!s)
	{
		debug("cstring not given\n");
		return NULL;
	}

    ms_sort(s->str);
    return s;
}
