#include <stdio.h>

#include "errors.h"
#include "parser.h"
#include "interpreter.h"
#include "gc.h"

int main(int argc, char *argv[])
{
    int ret;
    FILE *fp;
    Instruction start = {
        .instruction = I_NOP,
        .next_instruction = NULL,
        .alt_instruction = NULL
    };
    Variables global_vars;

    if (argc != 2 ||
        (fp = fopen(argv[1], "rb")) == NULL)
        return print_error(INTERNAL_ERROR);

    variables_init(&global_vars);

	debug("calling PARSE\n");
    if ((ret = parse(fp, &start, &global_vars)) == SUCCESS)
    {
		debug("PARSE ok\n\n");

#ifdef DEBUG
		debug("\n========================PRINTING GLOBAL VARIABLES==========================\n");
		variables_print(&global_vars);
		fprintf(stderr, "\n===========================================================================\n\n");
#endif

		debug("calling INTERPRETER\n");
		if ((ret = interpret(&start, &global_vars)) == SUCCESS)
		{
			debug("INTERPRETER OK\n");
		}
		else
		{
			debug("INTERPRETER: NOT success return code\n");
		}
	}
	else
	{
		debug("PARSER: NOT success return code\n");
	}

    fclose(fp);
    gc_free(GC_INSTR);
    variables_free(&global_vars);

	debug("MAIN RETURN CODE is: [ %d ]\n", ret);
    return print_error(ret);
}
