#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

void fail(const char *file, int line, const char *func, const char *expr) {
    printf("%%TEST_FAILED%% time=0 testname=%s (engine) message=failed %s at %s in %s:%d.\n", func, expr, func, file, line);
}
#define __ASSERT_FUNC __PRETTY_FUNCTION__
#define assert(__e) ((__e) ? (void)0 : fail(__FILE__, __LINE__, __ASSERT_FUNC, #__e))

void test_match() {
    AST *a = constr("A", 0);
    AST *a2 = constr("A", 0);
    AST *b = constr("B", 0);
    AST *c = constr("C", 2, a, b);
    assert(matches(a, a2));
    assert(!matches(a, b));
    assert(matches(c, c));
    assert(!matches(c, b));
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% engine\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% test_match (engine)\n");
    test_match();
    printf("%%TEST_FINISHED%% time=0 test_match (engine) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
