/**
 * @file    ial.h
 * @brief    Functions from Algorithms.
 * @author  Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01), Vojtech Mašek (xmasek15)
 ******************************************************************/

#ifndef __IAL_H__
#define __IAL_H__

#include <stdbool.h>

#include "cstring.h"

typedef struct Tree_Node {
    struct Tree_Node    *left;
    struct Tree_Node    *right;
    cstring             *key;
    void                *data;
}   Tree_Node;

typedef struct {
    Tree_Node   *root;
    Tree_Node   *last;
}   Tree;

typedef bool (*tree_function_ptr)(Tree_Node *node);

/**
 * @brief Creates Partial match table for Knuth–Morris–Pratt algorithm.
 *
 * @param word const char* String which is used for table construction.
 * @param _out int*  Table that is filled.
 */
void kmp_table(const char *word, int *_out);

/**
 * @brief Knuth–Morris–Pratt algorithm that searches for occurances of substring.
 *
 * @param  string String in which we search for substring.
 * @param  sub    Substring which we want to match.
 * @return int    On successful match returns beginning of a match, else -1.
 */
int kmp_substr(const char *string, const char *sub);


void merge_sort(char *string);


int         tree_insert(Tree *, cstring *, void *);
Tree_Node  *tree_find_key(Tree *, cstring *);
Tree_Node  *tree_find_key_ch(Tree *, char *);
Tree_Node  *tree_node_find(Tree_Node *, char *);
int         tree_create(Tree **tree);
int         tree_init(Tree *);
void        tree_free(Tree *);
void        tree_print(Tree *);


bool tree_check_all(Tree *tree, tree_function_ptr func);


#endif // __IAL_H__
