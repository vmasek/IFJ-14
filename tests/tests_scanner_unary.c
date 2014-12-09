#include "tests.h"

int unary(void);

void test_scanner_unary()
{
	unary();
}

int unary(void)
{
	int failures = 0;
	const int test_count = 7;

	printf("*** Testing Scanner unary operator ***\n");

	FILE *file;
	Token token;
	
	if( (file = fopen("unary.txt","r")) == NULL)
		{
			printf("Error while oppening file!\n");
			return RUNTIME_OTHER;
		}

	get_token(&token, file);
	debug("pred prvym\n");
	if(token.value.value_symbol != NEGATION )
	{
		failures++;
		debug("prvy\n");
		debug_token(&token);
	}

	debug("pred druhym\n");
	get_token(&token, file);
	if(token.type != TOKEN_INT)
	{
		failures++;
		debug("druhy\n");
		debug_token(&token);
	}

	debug("pred tretim\n");
	get_token(&token, file);
	if(token.value.value_symbol != SUBTRACTION)
	{
		failures++;
		debug("treti\n");
		debug_token(&token);
	}

	debug("pred stvrtym\n");
	get_token(&token, file);
	if(token.type != TOKEN_INT)
	{
		failures++;
		debug("stvrty\n");
		debug_token(&token);
	}
	
	debug("pred piatym\n");
	get_token(&token, file);
	if(token.value.value_symbol != SUBTRACTION )
	{
		failures++;
		debug("piaty\n");
		debug_token(&token);
	}
	
	debug("pred siestym\n");
		get_token(&token, file);
	if(token.value.value_symbol != NEGATION )
	{
		failures++;
		debug("siesty\n");
		debug_token(&token);
	}
	
	debug("pred siedmym\n");
	get_token(&token, file);
	

	
	if(token.type!= TOKEN_INT )
	{
		failures++;
		debug("siedmy\n");
		debug_token(&token);
	}

	print_result(test_count, failures);
	fclose(file);

	return failures;
}
