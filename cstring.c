/*
 * Nazev:   cstring.c
 * Popis:   Chybove hlasky
 * Autor:   Vojtech Mašek (xmasek15)
 ************************************************************/

#include    "errors.h"
#include    "cstring.h"
#include    <string.h>
#include    <stdlib.h>



/**
 * @brief Predlzenie cstringu
 * @param s ukazatel na cstring ktory bude predlzovany
 * @param str retazec o ktory sa predlzi cstring
 * @param size dlzka retazca o ktory budeme predlzovat
 * @returns ukazatel na predlzeny cstring s
 *
 * Funkcia vykonava predlzenie cstring retazca a zarovnen kontroluje
 * nevyziadane chovanie pri realokacii.
 */
static cstring *append(cstring *s, char const *str, ulong size)
{
    if (size >= s->tab_size && cstr_resize(s, size)) /** Kontrola dlzky, ak bude
    dlzka prekrocena zavola sa resize, iba ak budu obe podmienky vyhodnotene
    ako true (potrebna dlzka je vecsia a nepodari sa predlzenie) bude vrateny NULL */
        return NULL;
    if (s->size == 0) /** ak je retazec cstring prazdny */
        strcpy(s->str, str); /** tak sa str prekopiruje */
    else
        strcat(s->str, str); /** inak bude vlozeny na koniec */
    s->size = size;
    return s;
}


/**
 * @brief Spoji retazec s cstringom
 * @param s ukazatel na cstring ktory bude predlzeny
 * @param str retazec znakov ktory pripoji na koniec cstringu
 * @returns ukazatel na predlzeny cstring
 *
 * Pouziva staticky funkciu append implementovanu v cstring.c, blizsie detaily
 * uvedene priamo tam.
 */
cstring *cstr_append_str(cstring *s, char const *str)
{
    return append(s, str, s->size + strlen(str));
}


/**
 * @brief Spoji csting s cstringom
 * @param s ukazatel na cstring ktory bude predlzeny
 * @param cstr cstring ktory bude pripojeny na koniec cstringu s
 * @returns ukazatel na predlzeny cstring
 *
 * Pouziva staticky funkciu append implementovanu v cstring.c, blizsie detaily
 * uvedene priamo tam.
 */
cstring *cstr_append_cstr(cstring *s, cstring const *cstr)
{
    return append(s, cstr->str, s->size + cstr->size);
}


/**
 * @brief Priradi cstingu retazec
 * @param s ukazatel na cstring do ktoreho bude retazec priradeny
 * @param str retazec ktoru bude priradeny
 * @returns ukazatel na naplneny cstring s
 *
 * Pouziva staticky funkciu append implementovanu v cstring.c, blizsie detaily
 * uvedene priamo tam.
 */
cstring *cstr_assign_str(cstring *s, char const *str)
{
    cstr_clear(s);
    return append(s, str, strlen(str));
}


/**
 * @brief Priradi cstingu s cstring cstr
 * @param s ukazatel na cstring do ktoreho bude cstring cstr priradeny
 * @param cstr cstring ktory bude priradeny
 * @returns ukazatel na naplneny cstring s
 *
 * Pouziva staticky funkciu append implementovanu v cstring.c, blizsie detaily
 * uvedene priamo tam.
 */
cstring *cstr_assign_cstr(cstring *s, cstring const *cstr)
{
    cstr_clear(s);
    return append(s, cstr->str, cstr->size);
}


/**
 * @brief  Vytvori novy cstring.
 * @param  str [description]
 * @return ukazatel na novovytvoreny cstring
 *
 * Alokuje pamet pre retazec a vytvori strukturu.
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
 * @brief  Vytvori novy cstring.
 * @param  cstr ukazatel na cstring z ktoreho sa budu brat data
 * @return ukazatel na novovytvoreny cstring
 *
 * Obsah retazec z cstringu cstr ktory dostane parametrom zapise do novovytvoreneho
 *
 */
cstring *cstr_create_cstr(cstring const *cstr)
{
    cstring  *s = calloc(1, sizeof(*s));

    if (s)
        return cstr_append_cstr(s, cstr);

    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
}


/**
 * @brief Zvecsuje pamet pre cstring retazec
 * @param s cstring ktory potrebujeme realokovat
 * @param size minimalna velkost o ktoru je potrebne zvecsit
 * @returns Pozor vracia (-1) //TRUE ak realokovanie zlyhalo inak (0) //FALSE
 *
 *
 */
int cstr_resize(cstring *s, ulong size)
{
    /** Ak je cstring prazdny bude size nastavena na defaultnu hodnotu,
        inak je size2 velkost momentalnej naalokovanej dlzky */
    ulong size2 = s->tab_size ? s->tab_size : CSTRING_SIZE_START;
    char  *tmp;

    size2 *= (size + 1) / size2 + 1; /** zmensi alebo zvecsi size2 na potrebnu hodnotu */
    if (!(tmp = realloc(s->str, size2)))
        return -1;
    s->str = tmp;
    s->tab_size = size2;
    return 0;
}


/**
 * @brief Vlozi na zaciatok cstring retazca ukoncovaci znak a nastavi velkost na nulu.
 * @param s [description]
 */
void cstr_clear(cstring *s)
{
    s->str[0] = '\0';
    s->size = 0;
}


/**
 * @brief Uvolni pamet alokovanu pre data a strukturu cstringu.
 * @param s ukazatel na cstring
 */
void cstr_free(cstring *s)
{
    free(s->str);
    free(s);
}


/**
 * @brief Vypise retazec cstingu na samostany riadok.
 * @param s ukazatel na cstring
 */
void print_cstr(const cstring *s)
{
    //printf("%s\n",s->str);
    puts(s->str);
}


/**
 * @brief Vypise cely csting na samostany riadok.
 * @param s ukazatel na cstring
 *
 * Format : "size / capacity - [c_str()]"
 */
void print_cstr_all(const cstring *s)
{
    printf("\t%lu / %lu - [ %s ]\n", s->size, s->tab_size, s->str);
}


/*
//Testovaci main
int main()
{
    cstring*  str = cstr_create_str("Blablla");
    print_cstr_all(str);                                                // { 7 / 16 - [ Blablla ]   }
    print_cstr_all(cstr_assign_str(str, "LEET"));                       // { 4 / 16 - [ LEET ]  }
    print_cstr_all(cstr_append_str(str, " 1337"));                      // { 9 / 16 - [ LEET 1337 ] }
    cstr_clear(str);
    print_cstr_all(cstr_append_str(str, "@+!\t5%^*#42=answ2' '"));      // { 20 / 32 - [ @+! 5%^*#42=answ2' ' ] }

    cstr_free(str);

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

    cstr_free(str1);
    cstr_free(str2);

    return SUCCESS;
}
*/
