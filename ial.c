/**
 * @file	ial.h
 * @name	Implementation of functions from Algorithms.
 * @author	Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01)
 *******************************************************************/
 
#include "ial.h"

#include <stdio.h>
#include <stdlib.h>
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



/****************************************************************************/



void tree_init(Tree *tree)
{
	if(tree)
		tree->root = tree->last = NULL;
}



void tree_node_free(Tree_Node *node)
{
	printf("free");
	if(!node)
	{
		printf("not node\n");
		return;
	}

	if(node->left)
	{
		printf(" left\n");
		tree_node_free(node->left);
	}
	if(node->right)
	{
		printf(" right\n");
		tree_node_free(node->right);
	}

	//free(node->data);
	//toto bude pridane az ked sa budu allokovat data a nie pouzivany iba testovaci ukazatel na int
	free(node);
}

void tree_free(Tree *tree)
{
	if(!tree||!tree->root)
		return;

	tree_node_free(tree->root);

	tree_init(tree);
}


static inline int string_cmp( const char *str1, const char *str2)
{
	uint size1 = strlen(str1); /// TODO: optimize sizes (cstring has its size written)
	uint size2 = strlen(str2);

	if ( size1 < size2 )
		return 1; 				///LEFT
	else if ( size1 > size2 )
		return -1; 				///RIGHT

	return ((strcmp(str1, str2)==0) ? 0 : -1 );
}

Tree_Node *tree_find_node_recursive(Tree_Node *node, cstring *key)
{
	if(node == NULL)
        return NULL;
	else if(!strcmp(node->key->str, key->str))
		return node;
	else if (strlen(node->key->str) > strlen(key->str))
		return tree_find_node_recursive(node->left, key);
	else
		return tree_find_node_recursive(node->right, key);
}

Tree_Node *tree_node_find(Tree_Node *node, char *key)
{
	static int result;

	while(node)
	{
		printf("#\tkey: %s\tNode: %s\n",key, node->key->str);
		if( (result = string_cmp( key, node->key->str )) < 0 ) ///here was string_cmp replaced by classi strcmp that is fully functioning, string_cmp did not work
		{
			printf("right search\n");
			node = node->right;
		}
		else if( result > 0 )
		{
			printf("left search\n");
			node = node->left;
		}
		else /// node with same key as parameter key was found and will be returned
			return node;
	}

	return NULL;
}


Tree_Node *tree_find_key(Tree *tree, cstring *key)
{
	return tree_node_find(tree->root, key->str);
	//return tree_find_node_recursive(tree->root, key);
}

Tree_Node *tree_find_key_ch(Tree *tree, char *key)
{
	return tree_node_find(tree->root, key);
}

static inline void tree_create_root(Tree *tree, cstring *key, void *data)
{
	printf("root insert\n");
	tree->root = malloc(sizeof(Tree_Node));
	tree->root->key = key;
	tree->root->left = tree->root->right = NULL;
	tree->root->data = data;
	tree->last = tree->root;
}

int tree_insert(Tree *tree, cstring *key, void *data)
{
	if (!tree->root)
	{
		tree_create_root(tree, key, data);
		return 0;
	}

	uint key_size = strlen(key->str);

	Tree_Node *tmp = tree->root;
	uint result;

	while(tmp)
	{
		result = strlen(tmp->key->str);
		if (key_size < result)
		{
			printf("left insert\n");
			if(!tmp->left)
			{
				if(!(tmp->left = malloc(sizeof(Tree_Node))))
				{
					printf("malloc fail\n");
				}
				tmp->left->key = key;
				tmp->left->left = tmp->left->right = NULL;
				tmp->left->data = data;
				tree->last = tmp->left;
				return 0;
			}
			else
				tmp = tmp->left;
		}
		else if(key_size >= result)
		{
			printf("right insert\n");
			if (!tmp->right)
			{
				if(!(tmp->right = malloc(sizeof(Tree_Node))))
				{
					printf("malloc fail\n");
				}
				tmp->right->key = key;
				tmp->right->left = tmp->right->right = NULL;
				tmp->right->data = data;
				tree->last = tmp->right;
				return 0;
			}
			else
				tmp = tmp->right;
		}
		else
		{
			printf("last insert\n");
			tree->last = tmp;
			return 0;
		}
	}
	return 0;



}


void pre(Tree_Node *root)
{
    if(root != NULL){
        printf("%s ", root->key->str);
        pre(root->left);
        pre(root->right);
    }
}

void pre_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }
    printf("{");
    pre(t->root);
    printf("}");
    printf("\n");
}

void in(Tree_Node *root)
{
    if(root != NULL){
        in(root->left);
        printf("%s ", root->key->str);
        in(root->right);
    }
}

void in_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }

    printf("{");
    in(t->root);
    printf("}");
    printf("\n");
}

void post(Tree_Node *root)
{
    if(root != NULL){
        post(root->left);
        post(root->right);
        printf("%s ",root->key->str);
    }
}

void post_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }
    printf("{");
    post(t->root);
    printf("}");
    printf("\n");
}


void print_tree(Tree *tree)
{
	printf("PREORDEN\n");
    pre_orden(tree);

    printf("INORDEN\n");
    in_orden(tree);

    printf("POSTORDEN\n");
    post_orden(tree);
}









