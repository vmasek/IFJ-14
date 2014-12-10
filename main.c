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

    if ((ret = parse(fp, &start, &global_vars)) != SUCCESS)
        goto fail;

    if ((ret = interpret(&start, &global_vars)) != SUCCESS)
        goto fail;

fail:
    fclose(fp);
    gc_free(GC_INSTR);
    variables_free(&global_vars);

    return print_error(ret);
}
