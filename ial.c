/**
 * @file    ial.h
 * @brief   Implementation of functions from Algorithms.
 * @author  Albert Uchytil (xuchyt03), Pavel Tobias (xtobia01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "errors.h"
#include "ial.h"

#define TREE_LEFT 1
#define TREE_RIGT (-1)
#define TREE_ROOT 0

/* Returns lowest value of x1 and x2 */
#define MIN(x1, x2) ((x1) < (x2) ? (x1) : (x2))

/* Returns x if x is non-negative, otherwise 0 */
#define NN(x) ((x) > 0 ? (x) : 0)

/**
 * @brief Creates Partial match table for Knuth–Morris–Pratt algorithm.
 * @param word char* String which is used for table construction.
 * @param _out int*  Table that is filled.
 */
void kmp_table(const char *word, int *_out)
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
 * @param  string String in which we search for substring.
 * @param  sub    Substring which we want to match.
 * @return int    On successful match returns beginning of a match, else -1.
 */
int kmp_substr(const char *string, const char *sub)
{
    int sub_len = strlen(sub);
    int string_len = strlen(string);
    int curr_match = 0;     /* Index of current match that is getting checked. */
    int curr_position = 0;  /* Index of position within sub string. */
    int table[sub_len];

    debug("sub_len: %d\tstring_len: %d\n", sub_len, string_len);

    kmp_table(sub, table);

    debug("curr_match: %d\tcurr_position: %d\n", curr_match, curr_position);
    if (sub_len > 1) {
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
    } else if (sub_len == 1) {
        while (curr_position < string_len) {
            if (sub[0] == string[curr_position]) {
                return curr_position;
            }
            curr_position++;
        }
    }
    debug("substring not found\n");
    return -1; /* If we get here, occurance of substring was not found. */
}

/****************************************************************************/

static void merge(char **left, unsigned left_len, char **right,
                  unsigned right_len, char **dest, bool left2right)
{
    unsigned dest_len = left_len + right_len;

    while (dest_len--) {
        if (left_len && (!right_len || **left < **right)) {
            **dest = **left;
            (*left)++;
            left_len--;
        } else {
            **dest = **right;
            (*right)--;
            right_len--;
        }
        *dest += left2right ? 1 : -1;
    }
}

void merge_sort(char *string)
{
    unsigned string_len = strlen(string);
    char string_copy[string_len];
    char *source = string;
    char *dest = string_copy;
    char *source_left;
    char *source_right;
    char *dest_left;
    char *dest_right;
    bool left2right;

    if (string == NULL) {
        debug("No string given to sort");
        return;
    }

    for (unsigned seq_len = 1; seq_len < string_len; seq_len *= 2) {
        source_left = source;
        source_right = &source[string_len - 1];
        dest_left = dest;
        dest_right = &dest[string_len - 1];
        left2right = true;
        while (dest_left <= dest_right) {
            merge(&source_left,
                  MIN(seq_len, source_right - source_left + 1),
                  &source_right,
                  NN(MIN(seq_len, source_right - source_left + 1 - seq_len)),
                  left2right ? &dest_left : &dest_right,
                  left2right);
            left2right = !left2right;
        }
        source = (source == string) ? string_copy : string;
        dest = (dest == string) ? string_copy : string;
    }

    if (source == string_copy)
        memcpy(string, string_copy, string_len);
}

/****************************************************************************/

/**
 * @brief Inicialize binary tree.
 * @param pointer to tree to inicialize.
 */
int tree_init(Tree *tree)
{
	if (!tree)
    {
		debug("Can not init Null pointer.\n");
		return INTERNAL_ERROR;
	}

	tree->root = tree->last = NULL;

	return SUCCESS;
}


/**
 * @brief Creates tree.
 * @param pointer to pointer to tree to create.
 */
int tree_create(Tree **tree)
{
    if (!*tree)
    {
		*tree = (Tree *)malloc(sizeof(Tree));
	}

	if (!*tree)
    {
		return INTERNAL_ERROR;
	}

	(*tree)->root = (*tree)->last = NULL;

	return SUCCESS;
}


/**
 * @brief Function to free every node of tree.
 * @param node pointer to actual node
 *
 * Function recursiveli frees all nodes of binary tree.
 */
static void tree_node_free(Tree_Node *node)
{
    debug("free\n");
    if (!node) {
        debug("not node\n");
        return;
    }

    if (node->left) {
        debug("\tleft\n");
        tree_node_free(node->left);
    }
    if (node->right) {
        debug("\tright\n");
        tree_node_free(node->right);
    }

    //free(node->data);
    //toto bude pridane az ked sa budu allokovat data a nie pouzivany iba testovaci ukazatel na int
    free(node);
}


/**
 * @brief Frees whole binary tree.
 * @param tree to be freed.
 *
 * Calls function @see tree_node_free() that frees every node of binary tree.
 */
void tree_free(Tree *tree)
{
    if (!tree || !tree->root)
        return;

    tree_node_free(tree->root);

    tree_init(tree);
}


static inline int tree_string_cmp( const char *str, const cstring *cstr)
{
    unsigned int size = strlen(str); /// TODO: optimize sizes (cstring has its size written)

    if ( size < cstr->size )
        return 1;               ///LEFT
    else if ( size > cstr->size )
        return -1;              ///RIGHT

    return ((strcmp(str, cstr->str) == 0) ? 0 : -1 );
}


Tree_Node *tree_find_node_recursive(Tree_Node *node, cstring *key)
{
    if (node == NULL)
        return NULL;
    else if (!strcmp(node->key->str, key->str))
        return node;
    else if (node->key->size > key->size)
        return tree_find_node_recursive(node->left, key);
    else
        return tree_find_node_recursive(node->right, key);
}


Tree_Node *tree_node_find(Tree_Node *node, char *key)
{
    static int result;

    while (node) {
#ifdef DEBUG
        fprintf(stderr, "FOUND:\t\t#\tkey: %s\tNode: %s\n\n\n", key, node->key->str);
#endif
        if ( (result = tree_string_cmp( key, node->key )) < 0 ) { ///here was string_cmp replaced by classic strcmp that is fully functioning, string_cmp did not work
            debug("right search\n");
            node = node->right;
        } else if ( result > 0 ) {
            debug("left search\n");
            node = node->left;
        } else /// node with same key as parameter key was found and will be returned
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


static inline int tree_create_root(Tree *tree, cstring *key, void *data)
{
    debug("root insert\n");
    if (!(tree->root = malloc(sizeof(Tree_Node)))) {
        debug("malloc fail\n");
        return INTERNAL_ERROR;
    }
    tree->root->key = key;
    tree->root->left = tree->root->right = NULL;
    tree->root->data = data;
    tree->last = tree->root;
    return SUCCESS;
}


static inline int tree_create_left(Tree *tree, Tree_Node *tmp, cstring *key, void *data)
{
    debug("left insert\n");
    if (!(tmp->left = malloc(sizeof(Tree_Node)))) {
        debug("malloc fail\n");
        return INTERNAL_ERROR;
    }
    tmp->left->key = key;
    tmp->left->left = tmp->left->right = NULL;
    tmp->left->data = data;
    tree->last = tmp->left;
    return SUCCESS;
}


static inline int tree_create_right(Tree *tree, Tree_Node *tmp, cstring *key, void *data)
{
    debug("right insert\n");
    if (!(tmp->right = malloc(sizeof(Tree_Node)))) {
        debug("malloc fail\n");
        return INTERNAL_ERROR;
    }
    tmp->right->key = key;
    tmp->right->left = tmp->right->right = NULL;
    tmp->right->data = data;
    tree->last = tmp->right;
    return SUCCESS;
}


/**
 * @brief  Inerts node to tree.
 * @param  tree where to insert.
 * @param  key of node to search/insert.
 * @param  data
 * @return ERR. codes (UNDEFINED_IDENTIFIER when inserting existing key)
 */
int tree_insert(Tree *tree, cstring *key, void *data)
{
    if (!tree->root) {
        return tree_create_root(tree, key, data);
    }

    Tree_Node *tmp = tree->root;

    while (tmp) {


        if (key->size < tmp->key->size) {
            debug("left shift\n");
            if (!tmp->left) {
                return tree_create_left(tree, tmp, key, data);
            } else
                tmp = tmp->left;
        } else if (key->size > tmp->key->size) {
            debug("right shift\n");
            if (!tmp->right) {
                return tree_create_right(tree, tmp, key, data);
            } else
                tmp = tmp->right;
        } else {
            if (!cstr_cmp(key, tmp->key)) {
                return UNDEFINED_IDENTIFIER;
            }
            debug("right shift\n");
            if (!tmp->right) {
                return tree_create_right(tree, tmp, key, data);
            } else
                tmp = tmp->right;
        }
    }
    return SUCCESS;
}


static inline bool tree_recursive(Tree_Node *node, tree_function_ptr func)
{

    if (node->left)
    {
		debug("node->left\n");
		return tree_recursive(node->left, func);
	}

    if (node->right)
    {
		debug("node->right\n");
		return tree_recursive(node->right, func);
	}

	if(func(node))
	{
		debug("returning true\n");
		return true;
	}
	else
    {
		debug("returning false\n");
		return false;
	}
}


bool tree_check_all(Tree *tree, tree_function_ptr func)
{
	if (!tree||!tree->root)
	{
		debug("tree not given or tree does not have root (is empty or uninited), returning true\n");
		return true;
	}

	debug("calling tree recursive\n");
	return tree_recursive(tree->root, func);
}


static void tree_print_nodes(Tree_Node *node, const char *sufix, const char fromdir)
{
    if (node != NULL) {
        char *suf2 = (char *) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L') {
            suf2 = strcat(suf2, "  |");
            fprintf(stderr,"%s\n", suf2);
        } else
            suf2 = strcat(suf2, "   ");

        tree_print_nodes(node->right, suf2, 'R');
        fprintf(stderr,"%s  +-[%s]\n", sufix, node->key->str);
        strcpy(suf2, sufix);
        if (fromdir == 'R')
            suf2 = strcat(suf2, "  |");
        else
            suf2 = strcat(suf2, "   ");
        tree_print_nodes(node->left, suf2, 'L');
        if (fromdir == 'R') fprintf(stderr,"%s\n", suf2);
        free(suf2);
    }
}


/**
 * @brief Prints pretty scheme of binary tree.
 * @param tree to be printed.
 *
 * Used function of pretty print implemented in ial2 homework.
 * @see tree_print_nodes()
 */
void tree_print(Tree *tree)
{
    fprintf(stderr,"\n");
    if (tree != NULL)
        tree_print_nodes(tree->root, "", 'X');
    else
        fprintf(stderr, "Tree uninicialized\n");
    fprintf(stderr,"\n");
}



/*
static void pre(Tree_Node *root)
{
    if(root != NULL){
        printf("%s ", root->key->str);
        pre(root->left);
        pre(root->right);
    }
}

static void pre_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }
    printf("{");
    pre(t->root);
    printf("}\n");
}

static void in(Tree_Node *root)
{
    if(root != NULL){
        in(root->left);
        printf("%s ", root->key->str);
        in(root->right);
    }
}

static void in_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }

    printf("{");
    in(t->root);
    printf("}\n");
}

static void post(Tree_Node *root)
{
    if(root != NULL){
        post(root->left);
        post(root->right);
        printf("%s ",root->key->str);
    }
}

static void post_orden(Tree *t)
{
    if(t == NULL){
        printf("NULL\n");
        return;
    }
    printf("{");
    post(t->root);
    printf("}\n");
}


void tree_print(Tree *tree)
{
    printf("PREORDEN\n");
    pre_orden(tree);

    printf("INORDEN\n");
    in_orden(tree);

    printf("POSTORDEN\n");
    post_orden(tree);
}*/


