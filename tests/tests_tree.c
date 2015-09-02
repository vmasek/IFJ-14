/**
 *  @name   tests_tree.c
 *  @brief  Tests for tree
 *  @author Vojtech Mašek (xmasek15)
 ****************************************************************************/

#include "tests.h"



int test_tree(void)
{
	return test_tree_complex();
}

int test_tree_complex(void)
{
	static int errors = 1;

	Tree tree;
	tree_init(&tree);

do{ /// do{...}while(0); 	is for testing hack



#define _N_ 10
	const char *keys[_N_] = {"test", "test", "7357", "aaaaaaaaa", "aerhyearh", "4ae5sda64", "  asg  rrii", "zzzzzzz", "123456789", "wysdga"};
	cstring *cstr[_N_];
	Tree_Node *node;
	for(int i=0; i<_N_-1; i++)
	{
		cstr[i]=cstr_create_str(keys[i]);
		tree_insert(&tree, cstr[i], &i);
		node = tree_find_key(&tree, cstr[i]);
		if(!node)
		{
			printf("tERROR: searched node returned null\n");
			break;
		}
		//printf(":\t%s\t%d\n", node->key->str, *(int*)node->data);
		printf("Found:\t%s\n\n", node->key->str);
		//cstr_free(cstr[i]);
	}


	///tree_free(&tree); //ak chceme otestovat funkcnost free_tree v priebehu volania a nie len na konci


printf("\n---------------------------------\n");
	cstring *cstr0 = cstr_create_str("test");
	int data = 42;
	tree_insert(&tree, cstr0, &data);
	node = tree_find_key(&tree, cstr0);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	//printf(":\t%s\t%d\n", node->key->str, *(int*)node->data);
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr1 = cstr_create_str("7357");
	tree_insert(&tree, cstr1, &data);
	node = tree_find_key(&tree, cstr1);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr2 = cstr_create_str("xxx");
	tree_insert(&tree, cstr2, &data);
	node = tree_find_key(&tree, cstr2);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr3 = cstr_create_str("aaaaaaaaaaa");
	tree_insert(&tree, cstr3, &data);
	node = tree_find_key(&tree, cstr3);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr4 = cstr_create_str("a");
	tree_insert(&tree, cstr4, &data);
	node = tree_find_key(&tree, cstr4);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr5 = cstr_create_str("aab");
	tree_insert(&tree, cstr5, &data);
	node = tree_find_key(&tree, cstr5);
	if(!node)
	{
		printf("tERROR: searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);


	printf("\n\n--------------Print-tree---------------------\n\n");


	tree_print(&tree);


	printf("\n\n--------------free---------------------\n\n");


errors=0;
}while(0);


	tree_free(&tree);

	if(errors)
	{
		printf("\n\n!!! ERROR has occured during testing !!!\n\n");

	}
	else
	{
		printf("\n\nOK. Everything is fine.\n\n");
	}


	return errors;
}
