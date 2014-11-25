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
#include "../common.h"


///=================================BUILDIN===================================
int test_length(void);
int test_copy(void);
int test_find(void);
int test_sort(void);
void test_buildin(void);


///=================================COMMON====================================
void print_result(int, int);


///==================================IAL======================================
int test_kmp_substr(void);


///=================================STACK=====================================
int test_stack(void);
int test_stack_complex(void);

///==================================TREE=====================================
int test_tree(void);
int test_tree_complex(void);

///=================================CSTRING====================================
int test_cstring(void);
int test_tree_cstring(void);

#endif // __TESTS_H__
