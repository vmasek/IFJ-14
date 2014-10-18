/*
 * Nazev:   cstring.c
 * Popis:   Chybove hlasky
 * Autor:   Vojtech Mašek (xmasek15)
 ************************************************************/

#include    "errors.h"
#include    "cstring.h"
#include    <string.h>
#include    <stdlib.h>



static cstring *append(cstring *s, char const *str, ulong sz)
{
	if (sz >= s->tab_size && cstr_resize(s, sz))
		return NULL;
	(!s->size ? strcpy : strcat)(s->str, str);
	s->size = sz;
	return s;
}

/**
 * @brief Spoji retazec s cstringom
 * @param s ukazatel na cstring
 * @param str retazec znakov ktory pripoji na koniec cstringu
 * @returns ukazatel na predlzeny cstring
 *
 * Pouziva funkciu append implementovanu v cstring.c, blizsie detaily uvedene
 */
cstring *cstr_append_str(cstring *s, char const *str)
{
	return append(s, str, s->size + strlen(str));
}

cstring *cstr_append_cstr(cstring *s, cstring const *cstr)
{
	return append(s, cstr->str, s->size + cstr->size);
}

static cstring *assign(cstring *s, char const *cstr, ulong sz)
{
	if (sz >= s->tab_size && cstr_resize(s, sz))
		return NULL;
	strcpy(s->str, cstr);
	s->size = sz;
	return s;
}

cstring *cstr_assign_str(cstring *s, char const *str)
{
	return assign(s, str, strlen(str));
}

cstring *cstr_assign_cstr(cstring *s, cstring const *cstr)
{
	return assign(s, cstr->str, cstr->size);
}

/**
 * Vytvori novy cstring.
 * Alokuje pamet pre retazec a vytvori strukturu.
 *
 * @param  str [description]
 * @return     [description]
 */
cstring *cstr_create_str(char const *str)
{
	cstring  *s = calloc(1, sizeof(*s));

	if (s)
		return cstr_append_str(s, str);

	fprintf(stderr, "Memory allocation failed.\n");
	return NULL;
}

/**
 * Vytvori novy cstring.
 * Obsah cstringu ktory dostane parametrom zapise do novovytvoreneho
 *
 * @param  cstr [description]
 * @return     [description]
 */
cstring *cstr_create_cstr(cstring const *cstr)
{
	cstring  *s = calloc(1, sizeof(*s));

	if (s)
		return cstr_append_cstr(s, cstr);

	fprintf(stderr, "Memory allocation failed.\n");
	return NULL;
}

static cstring *insert(cstring *s, char const *cstr, ulong sz, ulong n)
{
	ulong new_sz = s->size + sz;

	if (new_sz >= s->tab_size && cstr_resize(s, new_sz))
		return NULL;
	memmove(s->str + n + sz, s->str + n, s->size + 1 - n);
	strncpy(s->str + n, cstr, sz);
	s->size = new_sz;
	return s;
}

cstring *cstr_insert_str(cstring *s, char const *str, ulong n)
{
	return n < s->size ? insert(s, str, strlen(str), n) :
		   cstr_append_str(s, str);

}

cstring *cstr_insert_cstr(cstring *s, cstring const *cstr, ulong n)
{
	return n < s->size ? insert(s, cstr->str, cstr->size, n) :
		   cstr_append_cstr(s, cstr);
}


int cstr_resize(cstring *s, ulong sz)
{
	ulong	sz2 = s->tab_size ? s->tab_size : CSTRING_SIZE_START;
	char	*tmp;

	sz2 *= (sz + 1) / sz2 + 1;
	if (!(tmp = realloc(s->str, sz2)))
		return -1;
	s->str = tmp;
	s->tab_size = sz2;
	return 0;
}

/**
 * Vlozi na zaciatok cstring retazca ukoncovaci znak a nastavi
 * velkost na nulu.
 * @param s [description]
 */
void cstr_clear(cstring *s)
{
	s->str[0] = '\0';
	s->size = 0;
}

/**
 * Uvolni pamet alokovanu pre data a strukturu cstringu.
 * @param s	ukazatel na cstring
 */
void cstr_destroy(cstring *s)
{
	free(s->str);
	free(s);
}

/**
 * Vypise obsah cstingu na samostany riadok.
 * @param s ukazatel na cstring
 */
void print_cstr(const cstring *s)
{
	//printf("%s\n",s->str);
	puts(s->str);
}
