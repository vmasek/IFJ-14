/**
 * @file    buildin.h
 * @name    Implementation of buildin functions.
 * @author  Tomas Paulus (xpaulu01)
 *******************************************************************/

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
int length(char *str)
{
    return strlen(str);
}

/**
 * @brief Copy characters from the character in the string at position i and copy
 * as much characters as the value of n is.
 *
 * @param  s char*  String from which is copied substring.
 * @param  i int    Index at which a substring starts.
 * @param  n int    Number of characters which are copied.
 * @return   char*  Returns copied substring.
 */
char *copy(char *s, int i, int n)
{
    /* TODO: Is it correct? */
    char *substr = malloc((n - i) * sizeof(char));
    char *originstr = malloc(strlen(s) * sizeof(char)); /* Copy of original string, which we dont want to edit */
    int position;

    originstr = s;

    /*TODO: Fix to return exact error_code if malloc isnt correct */
    for (position = 0 ; position < i ; position++)      /* Go to index position */
        originstr++;

    for (position = 0 ; position < (n - i) ; position++) { /* Copy characters */
        *(substr + position) = *originstr;
        originstr++;
    }
    *(substr + position) = '\0';

    return substr;
}

int find(char *s, char *search)
{
    return kmp_substr(s, search);
}

/**
 * @brief Sorting string characters by ordinal value.
 *
 * @param  s char* String which will be sorted.
 * @return   char* Sorted string.
 */
char *sort(char *s)
{
    char *sub = malloc(strlen(s) * sizeof(char));
    sub = s; /* Copy of original string, which we dont want to edit */
    ms_sort(sub);

    return sub;
}

/* Main for testing purposes
int main()
{
    char str[] = "1234567";
    char str2[] = "34";
    char str3[] = {'d', 'c', 'b', 'a'};
    printf("%d\n", length(str));
    printf("substring je %s \n", copy(str, 2, 4));
    printf("Nasiel som substring na pozicii %d\n", find(str, str2));
    printf("usporiadany string je %s \n", sort(str3));
    return 0;
}*/
