#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "assert.h"

void test_constructor_match() {
    Constructor a_ = {.symbol = "A", .length = 0};
    Term a = {.type = CONSTRUCTOR, .value.constructor = a_};
    assert(matches(&a, &a));

    Term b = {.type = CONSTRUCTOR, .value.constructor = (Constructor)
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
    Term a = {.type = CONSTANT, .value.constant = a_};
    assert(matches(&a, &a));

    Term b = {.type = CONSTANT, .value.constant = (Constant)
        { 3, "DEF"}};
    assert(!matches(&a, &b));
}

void test_constant_clone() {
    Constant a_ = {3, "ABC"};
    Term a = {.type = CONSTANT, .value.constant = a_};
    Term b;
    
    clone(&a, &b);
    assert(matches(&a, &b));
    assert(&a != &b);
}

void test_constructor_clone() {
    Term c = {.type = CONSTANT, .value.constant = {3, "ABC"}};
    Term w = {.type = WILDCARD, .value.wildcard = {}};
    Term *cs[2] = {&c, &w};
    
    Term a = {.type = CONSTRUCTOR, .value.constructor = {"A", 2, cs}};
    Term b;
    assert(a.value.constructor.children[0]->type == CONSTANT);
    
    clone(&a, &b);
    assert(matches(&a, &b));
    assert(&a != &b);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% engine\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_constructor_match);
    test(test_constant_match);
    test(test_constant_clone);
    test(test_constructor_clone);
   
    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
