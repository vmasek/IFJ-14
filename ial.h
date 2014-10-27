/**
 * @file	ial.h
 * @name	Functions from Algorithms.
 * @author	Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01)
 ******************************************************************/

#ifndef __IAL_H__
#define __IAL_H__

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Creates Partial match table for Knuth–Morris–Pratt algorithm.
 * 
 * @param word char* String which is used for table construction.
 * @param _out int*  Table that is filled.
 */
void kmp_table(char *word, int *_out);

/**
 * @brief Knuth–Morris–Pratt algorithm that searches for occurances of substring.
 * 
 * @param  string String in which we search for substring.
 * @param  sub    Substring which we want to match.
 * @return int    On successful match returns beginning of a match, else -1.
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
