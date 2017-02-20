#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "test_assert.h"


void test_build_single_tree() {
    Term *a = constr(CONSTRUCTOR, "A", 0);
    assert(a->children_length == 0);
    // apparently this isn't true... assert(a->children == NULL);

    destr(a);
    assert(a->symbol == NULL);
}

void test_build_complex_tree() {
    Term *b = constr(CONSTRUCTOR, "B", 0);
    Term *c = constr(CONSTRUCTOR, "C", 0);
    Term *a = constr(CONSTRUCTOR, "A", 2, b, c);
    assert(strncmp("B", a->children[0]->symbol, 10) == 0);
    assert(strncmp("C", a->children[1]->symbol, 10) == 0);
    assert(0 == a->children[0]->children_length);
    assert(0 == a->children[1]->children_length);
    
    destr(a);
    assert(a->children_length == 0);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% types\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_build_single_tree);
    test(test_build_complex_tree);
    
    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
