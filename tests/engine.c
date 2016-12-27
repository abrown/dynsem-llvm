#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "assert.h"

void test_match() {
    Constructor *a = constr("A", 0);
    Constructor *a2 = constr("A", 0);
    Constructor *b = constr("B", 0);
    Constructor *c = constr("C", 2, a, b);
//    assert(matches(a, a2));
//    assert(!matches(a, b));
//    assert(matches(c, c));
//    assert(!matches(c, b));
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
