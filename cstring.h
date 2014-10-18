/*
 * Nazev:   cstring.h
 * Popis:   Chybove hlasky
 * Autor:   Vojtech Mašek (xmasek15)
 **************************************************************/

#ifndef __CSTRING_H__
#define __CSTRING_H__

#include	<sys/types.h>

#define		CSTRING_SIZE_START  16

typedef struct
{
	char	*str;
	ulong	size, tab_size;
}	cstring;

/* Others */
void print_cstr(const cstring *s);


/* Allocation and copying */
cstring		*cstr_create_str(char const *);
cstring		*cstr_create_cstr(cstring const *);
int			cstr_resize(cstring *, ulong);



/* = ; += */
cstring		*cstr_assign_str(cstring *, char const *);
cstring		*cstr_assign_cstr(cstring *, cstring const *);
cstring		*cstr_append_str(cstring *, char const *);
cstring		*cstr_append_cstr(cstring *, cstring const *);
cstring		*cstr_insert_str(cstring *, char const *, ulong);
cstring		*cstr_insert_cstr(cstring *, cstring const *, ulong);


/* clean & free */
void		cstr_clear(cstring *);
void		cstr_destroy(cstring *);

#endif //__CSTRING_H__
