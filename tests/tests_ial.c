#include <stdio.h>
#include "tests_ial.h"
#include "tests_common.h"
#include "../ial.h"

/**
 * @brief Tests kmp_substr()
 * @ret   Number of failures
 */
int test_kmp_substr()
{
    int failures = 0;
    const int test_count = 5;
    printf("*** Testing KMP Substr ***\n");
    //TODO: make generic function
    if (kmp_substr("ahoj karle", "karl") != 5) {
        printf("ahoj karle, karl\n");
        failures++;
    }
    if (kmp_substr("123456", "123") != 0) {
        printf("12456, 123\n");
        failures++;
    }
    char a[] = "1234567";
    char b[] = "34";
    if (kmp_substr(a, b) != 2) {
        printf("%s, %s\n", a, b);
        failures++;
    }
    if (kmp_substr("1234", "43") != -1) {
        printf("1234, 43\n");
        failures++;
    }
    if (kmp_substr("", "54") != -1) {
        printf(", 54\n");
    }
    print_result(test_count, failures);

    return failures;
}
