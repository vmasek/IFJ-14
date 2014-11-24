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

do{ /// do{...}while(0); 	is for testing hack

    cstring*  str = cstr_create_str("Blablla");
	print_cstr_all(str);												// { 7 / 16 - [ Blablla ]	}
	print_cstr_all(cstr_assign_str(str, "LEET"));						// { 4 / 16 - [ LEET ]	}
    print_cstr_all(cstr_append_str(str, " 1337"));						// { 9 / 16 - [ LEET 1337 ]	}
    cstr_clear(str);
    print_cstr_all(cstr_append_str(str, "@+!\t5%^*#42=answ2' '"));		// { 20 / 32 - [ @+! 5%^*#42=answ2' ' ]	}

    cstring*  ch_str = cstr_create_str("11111111111");
    print_cstr_all(ch_str);
    cstr_clear(ch_str);
    print_cstr_all(ch_str);

    for (unsigned int i=0; i<20; i++)
    {
		print_cstr_all(cstr_append_chr(ch_str, 'A'));
		printf("\n");
	}


    cstring*  str1;
    cstring*  str2;

	printf("\n\n\n");
	//	{	131 / 144 - [ ... ]	}
	//	{	131 / 144 - [ ... ]	}
	//	{	626 / 720 - [ ... ]	}
	//	{	757 / 864 - [ ... ]	}

    print_cstr_all(str1 = cstr_create_str("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean et est a dui semper facilisis. Pellentesque placerat elit a nunc."));
    printf("\n\n");
    print_cstr_all(str2 = cstr_create_cstr(str1));
    printf("\n\n");
    print_cstr_all(cstr_assign_str(str1, "Nullam tortor odio, rutrum quis, egestas ut, posuere sed, felis. Vestibulum placerat feugiat nisl. Suspendisse lacinia, odio non feugiat vestibulum, sem erat blandit metus, ac nonummy magna odio pharetra felis. Vivamus vehicula velit non metus faucibus auctor. Nam sed augue. Donec orci. Cras eget diam et dolor dapibus sollicitudin. In lacinia, tellus vitae laoreet ultrices, lectus ligula dictum dui, eget condimentum velit dui vitae ante. Nulla nonummy augue nec pede. Pellentesque ut nulla. Donec at libero. Pellentesque at nisl ac nisi fermentum viverra. Praesent odio. Phasellus tincidunt diam ut ipsum. Donec eget est."));

    printf("\n\n");
    print_cstr_all(cstr_append_cstr(str2, str1));
    printf("\n\n");
    print_cstr_all(cstr_assign_str(str1, " & "));


	cstr_free(str);
    cstr_free(ch_str);
    cstr_free(str1);
    cstr_free(str2);



	Tree tree;
	tree_init(&tree);

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
			printf("searched node returned null\n");
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
		printf("searched node returned null\n");
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
		printf("searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr2 = cstr_create_str("xxx");
	tree_insert(&tree, cstr2, &data);
	node = tree_find_key(&tree, cstr2);
	if(!node)
	{
		printf("searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr3 = cstr_create_str("aaaaaaaaaaa");
	tree_insert(&tree, cstr3, &data);
	node = tree_find_key(&tree, cstr3);
	if(!node)
	{
		printf("searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr4 = cstr_create_str("a");
	tree_insert(&tree, cstr4, &data);
	node = tree_find_key(&tree, cstr4);
	if(!node)
	{
		printf("searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);

printf("\n---------------------------------\n");
	cstring *cstr5 = cstr_create_str("aab");
	tree_insert(&tree, cstr5, &data);
	node = tree_find_key(&tree, cstr5);
	if(!node)
	{
		printf("searched node returned null\n");
		break;
	}
	printf(":\t%s\n", node->key->str);


	printf("\n\n--------------Print-tree---------------------\n\n");


	tree_print(&tree);


	printf("\n\n--------------free---------------------\n\n");



	tree_free(&tree);

	for(int i=0; i<_N_-1; i++)
	{
		cstr_free(cstr[i]);
	}

	cstr_free(cstr0);
	cstr_free(cstr1);
	cstr_free(cstr2);
	cstr_free(cstr3);
	cstr_free(cstr4);
	cstr_free(cstr5);


errors=0;
}while(0);

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
