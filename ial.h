/**
 * @file	ial.h
 * @name	Functions from Algorithms.
 * @author	Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01)
 ******************************************************************/

#ifndef __IAL_H__
#define __IAL_H__

#include	"cstring.h"


#define MIN(a, b) ((a) < (b) ? (a) : (b))


typedef struct Tree_Node
{
	struct Tree_Node	*left;
	struct Tree_Node	*right;
	cstring				*key;
	void				*data;

}	Tree_Node;

typedef struct
{
	Tree_Node	*root;
	Tree_Node	*last;
}	Tree;



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



int			tree_insert(Tree *, cstring *, void *);
Tree_Node*	tree_find_key(Tree*, cstring*);
Tree_Node*	tree_find_key_ch(Tree*, char*);
Tree_Node*	tree_node_find(Tree_Node *, char *);
void		tree_init(Tree *);
void		tree_free(Tree *);

void print_tree(Tree *tree);


#endif // __IAL_H__
