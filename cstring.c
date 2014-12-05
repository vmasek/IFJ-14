/**
 * @file    cstring.c
 * @author  Vojtech Mašek (xmasek15)
 * @brief   Implementation of cstrig strings for IFJ projekt
 ****************************************************************************/

#include    "errors.h"
#include    "gc.h"
#include    "cstring.h"
#include    "common.h"
#include    <string.h>
#include    <stdlib.h>
#include    <ctype.h>


/**
 * @brief Extension of cstring.
 * @param s pointer to csting that will be extended.
 * @param str string that will be used to exted cstring.
 * @param size lenght of string "str".
 * @return pointer to extended cstring s.
 *
 * Function extends cstring controls the behavior of the unsolicited reallocation.
 */
static cstring *append(cstring *s, char const *str, unsigned long size)
{
	/*
	if(!s)
	{
		debug("cstring not given.");
		return NULL;
	}

	if(!str)
	{
		debug("str not given.");
		return NULL;
	}*/

    /**If size will be exceeded, resize will be called, only if both statements
     * are true (needed size is bigger and enlargement fails) NULL will be returned.
     * @code
     * if (size >= s->tab_size && cstr_resize(s, size))
     *     return NULL; @endcode
     */
	if (size >= s->tab_size && cstr_resize(s, size))
		return NULL;

    if (s->size == 0) // ak je retazec cstring prazdny
        strcpy(s->str, str); // tak sa str prekopiruje
    else
        strcat(s->str, str); // inak bude vlozeny na koniec

    s->size = size;
    return s;
}


/**
 * @brief Quick enlargement of memory allocated for strig in cstring.
 * @param s cstring that will be reallocated.
 * @return Warning returns (-1) //TRUE if realloc failed, else if everything is ok (0) //FALSE
 *
 * Reallocates memory for cstring string, multiples actual allocated size by 2.
 * Function is used for appending of chars to cstring, this will be used
 * frequently so it needst to be quick, and every delay by by reallocating
 * will be uselles.
 * Memory consumption was selected instead of time consumption, so there will
 * be no slowdowns.
 */
static int cstr_quick_resize(cstring *s)
{
    unsigned long new_size = s->tab_size * 2;
    char *tmp;
    //bude prealokovane na dvojnasobok aktualnej velkosti
    if (!(tmp = gc_realloc("cstring", s->str, new_size)))
        return -1;
    s->str = tmp;
    s->tab_size = new_size;
    return 0;
}


/**
 * @brief Appends char to cstring.
 * @param s pointer to cstring where char will be appended.
 * @param c char that will be appended.
 * @return cstrig with appended char.
 *
 * Function provides fast of chars at the end of the cstring, when reallocation
 * of memory is needed @see cstr_quick_resize() will be called, this method of
 * adding characters to a cstring chain is significantly faster than constantly
 * inserting string or reallocate always for one extra char.
 */
cstring *cstr_append_chr(cstring *s, char c)
{
	/** As long as false is evaluated in the first condition resize will not be called, the principe is essentially the same as for @see append() but with modifications excluding unnecessary operations */
    if (s->size + 1 >= s->tab_size && cstr_quick_resize(s))
        return NULL;

    s->str[s->size]      = c;
    s->str[s->size += 1] = '\0';
    return s;
}


/**
 * @brief Appends string to cstring.
 * @param s pointer to cstring where string will be appended.
 * @param str that will be appended.
 * @return cstrig with appended string.
 *
 * Uses static function @see append implemented in cstring.c, more details
 * written there.
 */
cstring *cstr_append_str(cstring *s, char const *str)
{
    return append(s, str, s->size + strlen(str));
}


/**
 * @brief Appends ctring to cstring
 * @param s pointer to cstring where cstring will be appended.
 * @param cstr that will be appended.
 * @return cstrig with appended cstring.
 *
 * Uses static function @see append implemented in cstring.c, more details
 * written there.
 */
cstring *cstr_append_cstr(cstring *s, cstring const *cstr)
{
    return append(s, cstr->str, s->size + cstr->size);
}


/**
 * @brief Assigns string to cstring.
 * @param s poiter to cstring where will be str assigned.
 * @param str string that will be assigned.
 * @return cstring
 *
 * Uses static function @see append implemented in cstring.c, more details
 * written there.
 */
cstring *cstr_assign_str(cstring *s, char const *str)
{
    cstr_clear(s);
    return append(s, str, strlen(str));
}


/**
 * @brief Assigns cstring to cstring.
 * @param s poiter to cstring where will be cstr assigned.
 * @param cstr cstring that will be assigned.
 * @return cstring
 *
 * Uses static function @see append implemented in cstring.c, more details
 * written there.
 */
cstring *cstr_assign_cstr(cstring *s, cstring const *cstr)
{
    cstr_clear(s);
    return append(s, cstr->str, cstr->size);
}


/**
 * @brief  Creates new cstring.
 * @param  str string that will be written into newly created cstring.
 * @return pointer to new cstring.
 *
 * Writes char from parameters into newly created cstring.
 */
cstring *cstr_create_chr(const char chr)
{
    cstring *s = gc_calloc("cstring", sizeof(cstring), 1);	///allocates new cstring with size of the CSTRING_START_SIZE

    if (!s)
    {
        fprintf(stderr, "Memory allocation for cstrig has failed.\n");
        return NULL;
    }

    s->str = gc_calloc("cstring", CSTRING_START_SIZE, 1);

    if (!s->str)
    {
        fprintf(stderr, "Memory allocation for cstrig string has failed.\n");
        return NULL;
    }

    s->tab_size = CSTRING_START_SIZE;
    s->size = 1;
    s->str[0] = chr;
    s->str[1] = '\0';

    return s;
}


/**
 * @brief  Creates new cstring.
 * @param  str string that will be written into newly created cstring.
 * @return pointer to new cstring.
 *
 * Writes str from parameters into newly created cstring.
 */
cstring *cstr_create_str(char const *str)
{
    cstring *s = gc_calloc("cstring", sizeof(*s), 1);

	if(!str)
	{
		debug("str not given.");
		return s;
	}


    if (s)
        return cstr_append_str(s, str);

    fprintf(stderr, "Memory allocation for cstrig has failed.\n");
    return NULL;
}


/**
 * @brief  Creates new cstring.
 * @param  cstr pointer to a cstring from which the data will be taken.
 * @return pointer to newly created cstring.
 *
 * Writes string from cstr parameter into newly created cstring.
 */
cstring *cstr_create_cstr(cstring const *cstr)
{
    cstring *s = gc_calloc("cstring", sizeof(*s), 1);

	if(!cstr)
	{
		debug("str not given.");
		return s;
	}

    if (s)
        return cstr_append_cstr(s, cstr);

    fprintf(stderr, "Memory allocation for cstrig has failed.\n");
    return NULL;
}


/**
 * @brief  Copies cstring.
 * @param  cstr pointer to cstring, from witch will be taken data.
 * @return pointer to created cstring.
 *
 * Warning: does not create exact copy of cstring, just copies "str" value and
 * creates new cstring with that value.
 */
cstring *cstr_copy(cstring const *cstr)
{
    return cstr_create_str(cstr->str);
}


/**
 * @brief Enlarges memory allocated for cstring string.
 * @param s cstring that will be reallocated.
 * @param size minimal size that needs to be added.
 * @return Warning returns (-1) //TRUE if realloc failed, else if everything is ok (0) //FALSE
 */
int cstr_resize(cstring *s, unsigned long size)
{
    /** If cstring is empty size will be set to default value,
        else is new_size set as size of new requied lenght */
    unsigned long new_size = s->tab_size ? size : CSTRING_START_SIZE;
    char  *tmp;

    new_size *= (size + 1) / new_size + 1; /** zmensi alebo zvecsi new_size na potrebnu hodnotu */
    if (!(tmp = gc_realloc("cstring", s->str, new_size)))
        return -1;
    s->str = tmp;
    s->tab_size = new_size;
    return 0;
}


/**
 * @brief Clears cstring.
 * @param s pointer to cstring that will be cleared.
 *
 * Inserts null terminate char to begin of the cstring and sets size to zero.
 */
void cstr_clear(cstring *s)
{
	if(!s)
	{
		debug("cstirng to clear not given.");
		return;
	}
    s->str[0] = '\0';
    s->size = 0;
}


/**
 * @brief Frees memory alocated for cstring.
 * @param s pointer to cstring.
 */
void cstr_gc_free_all(void)
{
    gc_free("cstring");
}


/**
 * @brief Prints cstring on one line.
 * @param s pointer to cstring.
 */
void print_cstr(cstring const *s)
{
    printf("%s",s->str);
    //puts(s->str);
}


/**
 * @brief Compares two cstrings.
 * @param s1 first cstring to compare
 * @param s2 second cstring to compare
 * @return int value, if cstrings are same returns 0, else <>0
 *
 * Uses strcmp to compare "str" valuses of cstrings.
 */
int cstr_cmp(cstring const *s1, cstring const *s2)
{
	if(!s1||!s2)
	{
		debug("one of cstrings to compare not given.");
	}

    return strcmp(s1->str, s2->str);
}

/**
 * @brief Compares cstring to string.
 * @param s1 cstring to compare
 * @param str string to compare
 * @return int value, if cstring and string are same returns 0, else <>0
 *
 * Uses strcmp to compare "str" value of cstring and string.
 */
int cstr_cmp_str(cstring const *s1, const char *str)
{
	if(!s1||!str)
	{
		debug("one of strings to compare not given.");
	}

    return strcmp(s1->str, str);
}

/**
 * @brief Lowers cstring chars.
 * @param s cstring to lower.
 */
void cstr_to_lower(cstring *s)
{
	if(!s)
	{
		debug("Cstring to lower not given.");
		return;
	}

	for(unsigned int i = 0; s->str[i] != '\0'; i++)
    {
        s->str[i]=tolower(s->str[i]);
    }
}


/**
 * @brief Prints whole cstring.
 * @param s pointer to cstring.
 *
 * Format : "size / capacity - [c_str()]"
 */
void print_cstr_all(cstring const *s)
{
	if(!s)
	{
		debug("cstirng to print not given.");
		return;
	}
    printf("\t%lu / %lu - [ %s ]\n", s->size, s->tab_size, s->str);
    for (unsigned int i = 0; i < s->size; i++) {
        printf("\t%d\t=\t%c\n", s->str[i], s->str[i]);
    }
}



cstring *cstr_read_line(cstring *cstr)
{
	if(!cstr)
		return NULL;

	int ch;
	cstr_clear(cstr);

	while(1)
	{
		ch = getchar();
		if(ch == '\n' || ch == EOF)
			return cstr;
		cstr_append_chr(cstr, ch);
	}
}
