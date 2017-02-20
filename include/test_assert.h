#include <stdio.h>
#include <time.h>

#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#define __ASSERT_FUNC __PRETTY_FUNCTION__

// macro for asserting a condition: assert(2 == 2);
void __test_assert_fail(const char *file, int line, const char *func, const char *expr);
#define assert(__e) ((__e) ? (void)0 : __test_assert_fail(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

// macro for running and timing a simple test case: test(my_test);
void __test_assert_test(void (*test)(), const char *test_name, const char *module_name);
#define test(f) __test_assert_test(f, #f, __FILE__)

#endif /* TEST_ASSERT_H */