
#include "tests.h"


int main(int argc, char *argv[])
{


	for(int i=1; i<argc || argc==1; i++)
	{
		if(argc == 1)
		{
			printf("***ALL TESTs will be runned***\n");
			argc=0;
		}
		if(!argc || !strcmp(argv[i], "debug"))									///nemusi byt pridane de test scriptu ale moze
		{
#ifndef DEBUG
			fprintf(stderr, "ERROR: Debug must be enabled to test Debug\n");
#endif
			debug("Test of DEBUGGING macro %s %s %s %d arguments. Some of them are %f and %d\n", "is OK.", "It has", "printed", 42, 1.6, 9999999);
		}
		if(!argc || !strcmp(argv[i], "kmp") )
			test_kmp_substr();
		if(!argc || !strcmp(argv[i], "stack"))
		{
			test_stack();
		}
		if(!argc || !strcmp(argv[i], "tree"))
		{
			test_tree();
		}
		if(!argc || !strcmp(argv[i], "cstring"))
		{
			test_cstring();
		}
		if(!argc || !strcmp(argv[i], "gc"))
		{
			test_gc();
		}
		if(!argc || !strcmp(argv[i], "buildin"))
		{
			test_buildin(); ///test for buildin copy gives SIGSEGV
		}

	}

	cstr_gc_free_all();
	return SUCCESS;
}
