#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "assert.h"

void test_constructor_match() {
    Term *a = constr(CONSTRUCTOR, "A", 0);
    assert(matches(a, a));

    Term *b = constr(CONSTRUCTOR, "B", 0);
    assert(!matches(a, b));

    Term *a2 = constr(CONSTRUCTOR, "A", 0);
    assert(matches(a, a2));
    
    Term *c = constr(CONSTANT, "A", 0);
    assert(!matches(a, c));
    
    Term *d = constr(CONSTRUCTOR, "A", 2, a, b);
    assert(!matches(a, d));
    
    destr(a2);
    destr(c);
    destr(d);
}

void test_constant_match() {
    Term *a = constr(CONSTANT, "ABC", 0);
    assert(matches(a, a));
    
    Term *b = constr(CONSTANT, "DEF", 0);
    assert(!matches(a, b));
    
    Term *c = constr(CONSTANT, "ABC", 1, b); // incorrect, but allowed by this permissive constr
    assert(matches(a, c));
    
    destr(a);
}

void test_constant_clone() {
    Term *a = constr(CONSTANT, "ABC", 0);
    Term *b = malloc(term_sizeof(a));

    clone(a, b);
    assert(matches(a, b));
    assert(a != b);
    
    destr(a);
    destr(b);
}

void test_constructor_clone() {
    Term *a = constr(CONSTRUCTOR, "A", 2, constr(CONSTANT, "ABC", 0), constr(WILDCARD, NULL, 0));
    Term *b = malloc(term_sizeof(a));
    assert(a->children[0]->tag == CONSTANT);

    clone(a, b);
    assert(matches(a, b));
    assert(a != b);
    
    destr(a);
    destr(b);
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
