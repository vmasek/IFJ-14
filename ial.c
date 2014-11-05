/**
 * @file	ial.h
 * @name	Implementation of functions from Algorithms.
 * @author	Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01)
 *******************************************************************/
 
#include "ial.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Creates Partial match table for Knuth–Morris–Pratt algorithm.
 * 
 * @param word char* String which is used for table construction.
 * @param _out int*  Table that is filled.
 */
void kmp_table(char *word, int *_out)
{
    int length_word = strlen(word); /* String length. */
    int table_position = 2;         /* Position within partial match table. */
    int candidate = 0;              /* Index of substring which is currently getting checked. */

    if (length_word >= table_position) {
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
 * @brief Knuth–Morris–Pratt algorithm that searches for occurances of substring.
 * 
 * @param  string String in which we search for substring.
 * @param  sub    Substring which we want to match.
 * @return int    On successful match returns beginning of a match, else -1.
 */
int kmp_substr(char *string, char *sub)
{
    int sub_len = strlen(sub);
    int string_len = strlen(string);
    int curr_match = 0;     /* Index of current match that is getting checked. */
    int curr_position = 0;  /* Index of position within sub string. */
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
    return -1; /* If we get here, occurance of substring was not found. */
}

/**
 * @brief Merging function for merge-sort.
 *
 * @param src    Source array of characters.
 * @param dst    Destination array where source array is merged.
 * @param len    Length of src/dst.
 * @param middle Initial index of right-hand sub-array.
 */
void ms_merge(const char *src, char *dst, unsigned len, unsigned middle)
{
    unsigned index_l = 0;
    unsigned index_r = middle;

    index_r = middle;

    for (unsigned index_dst = 0; index_dst < len; index_dst++) {
        if (index_r >= len || (index_l < middle &&
                               src[index_l] < src[index_r])) {
            dst[index_dst] = src[index_l];
            index_l++;
        } else {
            dst[index_dst] = src[index_r];
            index_r++;
        }
    }
}

/**
 * @brief Bottom-up implementation of merge-sort.
 * 
 * @param str Null-terminated character array to sort.
 */
void ms_sort(char *str)
{
    unsigned str_len = strlen(str);
    char buffer[str_len];
    char *src = str;
    char *dst = buffer;

    for (unsigned sub_len = 2; sub_len < str_len * 2; sub_len *= 2) {
        for (unsigned pos = 0; pos < str_len; pos += sub_len)
            ms_merge(src + pos, dst + pos, MIN(sub_len, str_len - pos),
                     sub_len / 2);
        ms_swap(&src, &dst);
    }

    if (src == buffer) {
        memcpy(str, buffer, str_len);
    }
}

/**
 * Pointer-swapping helper function for merge-sort.
 *
 * @param str1 Pointer to first pointer. 
 * @param str2 Pointer to second pointer.
 */
void ms_swap(char **str1, char **str2)
{
    char *swap_str = *str1;

    *str1 = *str2;
    *str2 = swap_str;
}
