/**
 * @file	cstring.h
 * @name	Chybove hlasky (header file)
 * @author	Vojtech Mašek (xmasek15)
 * @brief	Implementacia cstrig retazcov pre projekt do predmetu IFJ
 ****************************************************************************/

#ifndef __CSTRING_H__
#define __CSTRING_H__

#include    <sys/types.h>

#define     CSTRING_START_SIZE  16

typedef struct
{
    char    *str;
    ulong   size, tab_size;
}   cstring;

/* Print */
void        print_cstr(cstring const *);
void        print_cstr_all(cstring const *);


/* Allocation and copying */
cstring     *cstr_create_str(char const *);
cstring     *cstr_create_cstr(cstring const *);
int         cstr_resize(cstring *, ulong);



/* = and += */
cstring 	*cstr_append_chr(cstring *, char);

cstring     *cstr_assign_str(cstring *, char const *);
cstring     *cstr_append_str(cstring *, char const *);

cstring     *cstr_assign_cstr(cstring *, cstring const *);
cstring     *cstr_append_cstr(cstring *, cstring const *);


/* clean and free */
void        cstr_clear(cstring *);
void        cstr_free(cstring *);

#endif //__CSTRING_H__
