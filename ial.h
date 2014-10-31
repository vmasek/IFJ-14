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
 * @brief Merging function for merge-sort.
 *
 * @param src    Source array of characters.
 * @param dst    Destination array where source array is merged.
 * @param len    Length of src/dst.
 * @param middle Initial index of right-hand sub-array.
 */
void ms_merge(const char *src, char *dst, unsigned len, unsigned middle);

/**
 * @brief Bottom-up implementation of merge-sort.
 * 
 * @param str Null-terminated character array to sort.
 */
void ms_sort(char *str);

/**
 * Pointer-swapping helper function for merge-sort.
 *
 * @param str1 Pointer to first pointer. 
 * @param str2 Pointer to second pointer.
 */
void ms_swap(char **str1, char **str2);


#endif // __IAL_H__
