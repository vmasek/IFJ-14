/***********************************************
 * Popis:   Rozhrani poskytujici funkce z predmetu IAL
 * Nazev:   ial.c
 * Autori:  Albert Uchytil (xuchyt03)
 ***********************************************/

#ifndef __IAL_H__
#define __IAL_H__

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

#endif // __IAL_H__
