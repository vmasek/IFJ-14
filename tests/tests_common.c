#include <stdio.h>
#include "tests_common.h"

void print_result(int test_count, int failures) {
    printf("Result: Failed %d of %d\n", failures, test_count);
}

