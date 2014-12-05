#include <stdio.h>

#include "errors.h"
#include "parser.h"

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

    variables_init(&global_vars);

    if (argc < 2) {
        return print_error(INTERNAL_ERROR);
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        return print_error(INTERNAL_ERROR);
    }

    ret = print_error(parse(fp, &start, &global_vars));

    fclose(fp);

    //TODO interpretation here

    variables_free(&global_vars);

    return ret;
}
