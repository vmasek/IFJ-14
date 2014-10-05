/***********************************************
 * Popis:   Rozhrani poskytujici funkce z predmetu IAL
 * Nazev:   ial.c
 * Autori:  Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01)
 ***********************************************/

#ifndef __IAL_H__
#define __IAL_H__

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * Pomocna tabulka pro Knuth-Morris-Prattuv algoritmus.
 * 
 * @param word char* Retezec, ze ktereho je vypocitana pomocna tabulka.
 * @param _out int*  Pomocna tabulka, ktera je naplnena.
 */
void kmp_table(char *word, int *_out);

/**
 * Knuth-Morris-Prattuv algoritmus pro vyhledavani podretezce v retezci.
 * 
 * @param  string Retezec, ve kterem je provadeno vyhledavani.
 * @param  sub    Podretezec, ktery je vyhledavan.
 * @return int    V pripade vyskytu navraci index zacatku substringu, jinak -1.
 */
int kmp_substr(char *string, char *sub);

/**
 * Rozdeleni pole znaku na 2 sub-pole a jejich slouceni (merge)
 *
 * @param src const char* Zdrojove pole znaku.
 * @param dst char* Cilove pole znaku, kam je slouceno zdrojove pole.
 * @param len unsigned Delka zdrojoveho/ciloveho pole.
 * @param middle unsigned Pocatecni index praveho sub-pole.
 */
void ms_merge(const char *src, char *dst, unsigned len, unsigned middle);

/**
 * Bottom-up implementace merge-sortu
 *
 * @param str char* Retezec k serazeni.
 */
void ms_sort(char *str);

/**
 * Vzajemne premireni 2 ukazatelu na char
 *
 * @param str1 char** Ukazatel na prvniho ukazatele. 
 * @param str2 char** Ukazatel na druheho ukazatele.
 */
void ms_swap(char **str1, char **str2);


#endif // __IAL_H__
