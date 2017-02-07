#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "assert.h"

void test_constructor_match() {
    Constructor a_ = {.symbol = "A", .length = 0};
    AST a = {.type = CONSTRUCTOR, .value.constructor = a_};
    assert(matches(&a, &a));

    AST b = {.type = CONSTRUCTOR, .value.constructor = (Constructor)
        { .symbol = "B", .length = 0}};
    assert(!matches(&a, &b));


    //    Constructor *a = constr("A", 0);
    //    Constructor *a2 = constr("A", 0);
    //    Constructor *b = constr("B", 0);
    //    Constructor *c = constr("C", 2, a, b);
    //    assert(matches(a, a2));
    //    assert(!matches(a, b));
    //    assert(matches(c, c));
    //    assert(!matches(c, b));
}

void test_constant_match() {
    Constant a_ = {3, "ABC"};
    AST a = {.type = CONSTANT, .value.constant = a_};
    assert(matches(&a, &a));

    AST b = {.type = CONSTANT, .value.constant = (Constant)
        { 3, "DEF"}};
    assert(!matches(&a, &b));
}

void test_constant_clone() {
    
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% engine\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_constructor_match);
    test(test_constant_match);
    test(test_constant_clone);
   
    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
