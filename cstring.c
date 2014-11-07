/**
 * @file    cstring.c
 * @name    Chybove hlasky
 * @author  Vojtech Mašek (xmasek15)
 * @brief   Implementacia cstrig retazcov pre projekt do predmetu IFJ
 ****************************************************************************/

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
    /**Kontrola dlzky, ak bude dlzka prekrocena zavola sa resize, iba ak budu
     * obe podmienky vyhodnotene ako true (potrebna dlzka je vecsia a nepodari
     * sa predlzenie) bude vrateny NULL.
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
 * @brief Rychle znasobenie pamete pre retazec v cstringu
 * @param s cstring ktory bude realokovany
 * @returns Pozor vracia (-1) //TRUE ak realokovanie zlyhalo inak (0) //FALSE
 *
 * Realokuje pamet pre cstring retazec tak ze znasobi aktualne alokovane
 * mnozstvo dvomi.
 * Funkcia vyuzivana pri appendovani charov do cstringu, tento proces bude
 * casty a potrebujeme aby nebol zdrzovany neustalim realokovanim zdrojov.
 * Bola tak zvolena pametova narocnost na ukor vyssej rychlosti.
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
 * @brief Prida char k cstring retazcu
 * @param s ukazadel na cstring kde chceme pridat char.
 * @param c char ktory bude appendnuty
 * @returns ukazatel na cstring s pridanym charom
 *
 * Funkcia ktora zabezpecuje moznost rychleho pridavania charov na koniec
 * cstring retazca, ak je potrebna realokacia pamete pre retazec, tak vola
 * funkciu @see cstr_quick_resize(), tento sposob pridavania znakov do cstring
 * retazca je vyrazne rychlejsi ako neustale vkladanie string alebo realokacia
 * vzdy pre jeden char naviac.
 */
cstring *cstr_append_chr(cstring *s, char c)
{
    ///pokial bude ako false vyhodnotena uz prva podmienka tak sa resize ani volat nebude, princip v podstate rovnaky ako pri @see append() ale s upravami vynechavajucimi zbytocne operacie
    if (s->size + 1 >= s->tab_size && cstr_quick_resize(s))
        return NULL;

    s->str[s->size] = c;
    s->str[s->size += 1] = '\0';
    return s;
}

/**
 * @brief Spoji retazec s cstringom
 * @param s ukazatel na cstring ktory bude predlzeny
 * @param str retazec znakov ktory pripoji na koniec cstringu
 * @returns ukazatel na predlzeny cstring
 *
 * Pouziva staticku funkciu append implementovanu v cstring.c, blizsie detaily
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
 * Pouziva staticku funkciu append implementovanu v cstring.c, blizsie detaily
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
 * @see append()
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
    cstring *s = calloc(1, sizeof(*s));

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
 * @brief Zvecsuje pamet pre cstring retazec
 * @param s cstring ktory potrebujeme realokovat
 * @param size minimalna velkost o ktoru je potrebne zvecsit
 * @returns Pozor vracia (-1) //TRUE ak realokovanie zlyhalo inak (0) //FALSE
 */
int cstr_resize(cstring *s, ulong size)
{
    /** Ak je cstring prazdny bude size nastavena na defaultnu hodnotu,
        inak je new_size velkost momentalnej naalokovanej dlzky */
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
 * @brief Vlozi na zaciatok cstring retazca ukoncovaci znak a nastavi velkost na nulu.
 * @param s ukazatel na cstring ktory bude vycisteny
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
 * @brief Vypise cely csting na samostany riadok.
 * @param s ukazatel na cstring
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
int main()
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


    cstr_free(str);
    cstr_free(ch_str);
    cstr_free(str1);
    cstr_free(str2);

    return SUCCESS;
}
*/
