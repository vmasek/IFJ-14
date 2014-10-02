/***********************************************
 * Popis:   Implementace funkci z predmetu IAL
 * Nazev:   ial.c
 * Autori:  Albert Uchytil (xuchyt03)
 ***********************************************/
 
#include "ial.h"

#include <stdio.h>
#include <string.h>

/**
 * Pomocna tabulka pro Knuth-Morris-Prattuv algoritmus.
 * 
 * @param word char* Retezec, ze ktereho je vypocitana pomocna tabulka.
 * @param _out int*  Pomocna tabulka, ktera je naplnena.
 */
void kmp_table(char *word, int *_out)
{
    int length_word = strlen(word); /* Delka retezce. */
    int table_position = 2;         /* Pozice v pomocne tabulce. */
    int candidate = 0;              /* Index aktualne zkoumaneho podretezce */

    if (length_word > table_position) {
        _out[0] = -1;
        _out[1] = 0;
    }

    while (table_position < length_word) {
        if (word[table_position - 1] == word[candidate]) {
            candidate++;
            _out[table_position] = candidate;
            table_position++;
        } else if (candidate > 0 ) {
            candidate = _out[candidate];
        } else {
            _out[table_position] = 0;
            table_position++;
        }
    }
}

/**
 * Knuth-Morris-Prattuv algoritmus pro vyhledavani podretezce v retezci.
 * 
 * @param  string Retezec, ve kterem je provadeno vyhledavani.
 * @param  sub    Podretezec, ktery je vyhledavan.
 * @return int    V pripade vyskytu navraci index zacatku substringu, jinak -1.
 */
int kmp_substr(char *string, char *sub)
{
    int sub_len = strlen(sub);
    int string_len = strlen(string);
    int curr_match = 0;     /* Index zacatku aktualne prosetrovane shody. */
    int curr_position = 0;  /* Index pozice v retezci sub. */
    int table[sub_len];
    kmp_table(sub, table);

    while (curr_match + curr_position < string_len) {
        if (sub[curr_position] == string[curr_match + curr_position]) {
            if (curr_position == sub_len - 1) {
                return curr_match;
            }
            curr_position++;
        } else {
            if (table[curr_position] > -1) {
                curr_match = curr_match + curr_position - table[curr_position];
                curr_position = table[curr_position];
            } else {
                curr_position = 0;
                curr_match++;
            }
        }
    }
    return -1; /* Pokud se dostaneme sem, vyskyt podretezce nebyl nalezen. */
}
