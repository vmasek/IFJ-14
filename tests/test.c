
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
		if(!argc || !strcmp(argv[i], "kmp") )
			test_kmp_substr();
		if(!argc || !strcmp(argv[i], "stack"))
		{
			test_stack();
		}
		if(!argc || !strcmp(argv[i], "buildin"))
			test_buildin(); ///test for buildin copy gives SIGSEGV
	}
}
