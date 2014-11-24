/**
 *  @name   tests.h
 *  @brief  Tests (header)
 ****************************************************************************/

#ifndef __TESTS_H__
#define __TESTS_H__

#include <string.h>
#include <stdio.h>

#include "../errors.h"
#include "../cstring.h"
#include "../stack.h"
#include "../ial.h"
#include "../buildin.h"


///=================================BUILDIN===================================
int test_length();
int test_copy();
int test_find();
int test_sort();
void test_buildin();


///=================================COMMON====================================
void print_result(int test_count, int failures);


///==================================IAL======================================
int test_kmp_substr();


///=================================STACK=====================================
int test_stack(void);
int test_stack_complex(void);


#endif // __TESTS_H__
