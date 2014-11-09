/**
 * @file    cstring.c
 * @name    Cstring
 * @author  Vojtech Mašek (xmasek15)
 * @brief   Implementation of cstrig strings for IFJ projekt
 ****************************************************************************/

#include    "errors.h"
#include    "cstring.h"
#include    <string.h>
#include    <stdlib.h>


/**
 * @brief Extension of cstring.
 * @param s pointer to csting that will be extended.
 * @param str string that will be used to exted cstring.
 * @param size lenght of string "str".
 * @returns pointer to extended cstring s.
 *
 * Function extends cstring controls the behavior of the unsolicited reallocation.
 */
static cstring *append(cstring *s, char const *str, ulong size)
{
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
 * @returns Warning returns (-1) //TRUE if realloc failed, else if everything is ok (0) //FALSE
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
    ulong new_size = s->tab_size * 2;
    char *tmp;
    //bude prealokovane na dvojnasobok aktualnej velkosti
    if (!(tmp = realloc(s->str, new_size)))
        return -1;
    s->str = tmp;
    s->tab_size = new_size;
    return 0;
}


/**
 * @brief Appends char to cstring.
 * @param s pointer to cstring where char will be appended.
 * @param c char that will be appended.
 * @returns cstrig with appended char.
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

    s->str[s->size] = c;
    s->str[s->size += 1] = '\0';
    return s;
}


/**
 * @brief Appends string to cstring.
 * @param s pointer to cstring where string will be appended.
 * @param str that will be appended.
 * @returns cstrig with appended string.
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
 * @returns cstrig with appended cstring.
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
 * @returns cstring
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
 * @returns cstring
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
 * Writes str from parameters into newly created cstring.
 */
cstring *cstr_create_str(char const *str)
{
    cstring *s = calloc(1, sizeof(*s));

    if (s)
        return cstr_append_str(s, str);

    fprintf(stderr, "Memory allocation failed.\n");
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
    cstring *s = calloc(1, sizeof(*s));

    if (s)
        return cstr_append_cstr(s, cstr);

    fprintf(stderr, "Memory allocation failed.\n");
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
 * @returns Warning returns (-1) //TRUE if realloc failed, else if everything is ok (0) //FALSE
 */
int cstr_resize(cstring *s, ulong size)
{
    /** If cstring is empty size will be set to default value,
        else is new_size set as size of new requied lenght */
    ulong new_size = s->tab_size ? s->tab_size : CSTRING_START_SIZE;
    char  *tmp;

    new_size *= (size + 1) / new_size + 1; /** zmensi alebo zvecsi new_size na potrebnu hodnotu */
    if (!(tmp = realloc(s->str, new_size)))
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
    s->str[0] = '\0';
    s->size = 0;
}


/**
 * @brief Frees memory alocated for cstring.
 * @param s pointer to cstring.
 */
void cstr_free(cstring *s)
{
    free(s->str);
    free(s);
}


/**
 * @brief Prints cstring on one line.
 * @param s pointer to cstring.
 */
void print_cstr(cstring const *s)
{
    //printf("%s\n",s->str);
    puts(s->str);
}

/**
 * @brief Compares two cstrings.
 * @param s1 first cstring to compare
 * @param s2 second cstring to compare
 * @returns int value, if cstrings are same returns 0, else <>0
 *
 * Uses strcmp to compare "str" valuses of cstrings.
 */
int cstr_cmp(cstring const *s1, cstring const *s2)
{
    return strcmp(s1->str, s2->str);
}


/**
 * @brief Prints whole cstring.
 * @param s pointer to cstring.
 *
 * Format : "size / capacity - [c_str()]"
 */
void print_cstr_all(cstring const *s)
{
    printf("\t%lu / %lu - [ %s ]\n", s->size, s->tab_size, s->str);
    for (uint i = 0; i < s->size; i++) {
        printf("\t%d\t=\t%c\n", s->str[i], s->str[i]);
    }
}


/*
//----------------------------Testovaci-main----------------------------------
int main(void)
{
    cstring*  str = cstr_create_str("Blablla");
    print_cstr_all(str);                                                // { 7 / 16 - [ Blablla ]   }
    print_cstr_all(cstr_assign_str(str, "LEET"));                       // { 4 / 16 - [ LEET ]  }
    print_cstr_all(cstr_append_str(str, " 1337"));                      // { 9 / 16 - [ LEET 1337 ] }
    cstr_clear(str);
    print_cstr_all(cstr_append_str(str, "@+!\t5%^*#42=answ2' '"));      // { 20 / 32 - [ @+! 5%^*#42=answ2' ' ] }

    cstring*  ch_str = cstr_create_str("11111111111");
    print_cstr_all(ch_str);
    cstr_clear(ch_str);
    print_cstr_all(ch_str);

    for (uint i=0; i<20; i++)
    {
        print_cstr_all(cstr_append_chr(ch_str, 'A'));
        printf("\n");
    }


    cstring*  str1;
    cstring*  str2;

    printf("\n\n\n");
    //  {   131 / 144 - [ ... ] }
    //  {   131 / 144 - [ ... ] }
    //  {   626 / 720 - [ ... ] }
    //  {   757 / 864 - [ ... ] }

    print_cstr_all(str1 = cstr_create_str("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean et est a dui semper facilisis. Pellentesque placerat elit a nunc."));
    printf("\n\n");
    print_cstr_all(str2 = cstr_create_cstr(str1));
    printf("\n\n");
    print_cstr_all(cstr_assign_str(str1, "Nullam tortor odio, rutrum quis, egestas ut, posuere sed, felis. Vestibulum placerat feugiat nisl. Suspendisse lacinia, odio non feugiat vestibulum, sem erat blandit metus, ac nonummy magna odio pharetra felis. Vivamus vehicula velit non metus faucibus auctor. Nam sed augue. Donec orci. Cras eget diam et dolor dapibus sollicitudin. In lacinia, tellus vitae laoreet ultrices, lectus ligula dictum dui, eget condimentum velit dui vitae ante. Nulla nonummy augue nec pede. Pellentesque ut nulla. Donec at libero. Pellentesque at nisl ac nisi fermentum viverra. Praesent odio. Phasellus tincidunt diam ut ipsum. Donec eget est."));

    printf("\n\n");
    print_cstr_all(cstr_append_cstr(str2, str1));
    printf("\n\n");
    print_cstr_all(cstr_assign_str(str1, " & "));


	if(cstr_cmp(str, str1))
	{
		printf("\n1. compare ok\n");
	}
	else
	{
		printf("\n1. compare fail\n");
	}

	print_cstr_all(str = cstr_assign_str(str, "aaaaaaaaaaaaaaaaaaaaaaa\n"));
	print_cstr_all(str = cstr_assign_cstr(str, str1));

	if(!cstr_cmp(str, str1))
	{
		printf("\n2. compare ok\n");
	}
	else
	{
		printf("\n2. compare fail\n");
	}

    cstr_free(str);
    cstr_free(ch_str);
    cstr_free(str1);
    cstr_free(str2);


    return SUCCESS;
}
*/
