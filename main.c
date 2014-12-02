#include <stdio.h>

#include "errors.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc < 2) {
        return print_error(INTERNAL_ERROR);
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        return print_error(INTERNAL_ERROR);
    }

    return print_error(parse(fp));
}
