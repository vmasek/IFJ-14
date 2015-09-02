/**
 *  @name   tests_cstring.c
 *  @brief  Tests for cstring
 *  @author Vojtech Mašek (xmasek15)
 ****************************************************************************/

#include "tests.h"



int test_cstring(void)
{
	return test_cstring_complex();
}

int test_cstring_complex(void)
{
	static int errors = 1;


do{ /// do{...}while(0); 	is for testing hack

    cstring* str = cstr_create_str(NULL);
    if(str)
	{
		printf("\n0. OK\n");
	}
	else
	{
		printf("\n0. Fail. Returned Null.\n");
		break;
	}
	printf("\n");

    str = cstr_create_str("Blablla");
    printf("aaaaaaaa\n");
    //print_cstr_all(str);                                                // { 7 / 16 - [ Blablla ]   }
    /*print_cstr_all(*/cstr_assign_str(str, "LEET")/*)*/;                       // { 4 / 16 - [ LEET ]  }
    /*print_cstr_all(*/cstr_append_str(str, " 1337")/*)*/;                      // { 9 / 16 - [ LEET 1337 ] }
    /*print_cstr_all(*/cstr_append_str(str, " dalsi string")/*)*/;
    cstr_clear(str);
    cstr_append_str(str, "@+!\t5%^*#42=answ2' '");      // { 20 / 32 - [ @+! 5%^*#42=answ2' ' ] }

    cstring*  ch_str = cstr_create_str("11111");
    /*print_cstr_all(ch_str);*/
    cstr_clear(ch_str);
    /*print_cstr_all(ch_str);*/

    for (unsigned int i=0; i<20; i++)
    {
        //print_cstr_all(cstr_append_chr(ch_str, 'A'));
        cstr_append_chr(ch_str, 'A');
        //printf("\n");
    }


    cstring*  str1;
    cstring*  str2;

    //  {   131 / 144 - [ ... ] }
    //  {   131 / 144 - [ ... ] }
    //  {   626 / 720 - [ ... ] }
    //  {   757 / 864 - [ ... ] }

    str1 = cstr_create_str("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean et est a dui semper facilisis. Pellentesque placerat elit a nunc.");

    str2 = cstr_create_cstr(str1);

    cstr_assign_str(str1, "Nullam tortor odio, rutrum quis, egestas ut, posuere sed, felis. Vestibulum placerat feugiat nisl. Suspendisse lacinia, odio non feugiat vestibulum, sem erat blandit metus, ac nonummy magna odio pharetra felis. Vivamus vehicula velit non metus faucibus auctor. Nam sed augue. Donec orci. Cras eget diam et dolor dapibus sollicitudin. In lacinia, tellus vitae laoreet ultrices, lectus ligula dictum dui, eget condimentum velit dui vitae ante. Nulla nonummy augue nec pede. Pellentesque ut nulla. Donec at libero. Pellentesque at nisl ac nisi fermentum viverra. Praesent odio. Phasellus tincidunt diam ut ipsum. Donec eget est.");

    cstr_append_cstr(str2, str1);

    cstr_assign_str(str1, " & ");


	if(cstr_cmp(str, str1))
	{
		printf("\n1. compare ok\n");
	}
	else
	{
		printf("\n1. compare fail\n");
		break;
	}

	str = cstr_assign_str(str, "aaaaaaaaaaaaaaaaaaaaaaa\n");
	str = cstr_assign_cstr(str, str1);

	if(!cstr_cmp(str, str1))
	{
		printf("\n2. compare ok\n");
	}
	else
	{
		printf("\n2. compare fail\n");
		break;
	}



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
