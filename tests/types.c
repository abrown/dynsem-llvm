#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "assert.h"


void test_build_single_tree() {
    Constructor *a = constr("A", 0);
    assert(a->length == 0);
    assert(a->children == NULL);

    destr(a);
    assert(a->symbol == NULL);
}

void test_build_complex_tree() {
    AST b = { .type = CONSTRUCTOR, .value.constructor = { "B", 0, NULL } };
    AST c = { .type = CONSTRUCTOR, .value.constructor = { "C", 0, NULL } };
    Constructor *a = constr("A", 2, &b, &c);
    assert(strncmp("B", a->children[0]->value.constructor.symbol, 10) == 0);
    assert(strncmp("C", a->children[1]->value.constructor.symbol, 10) == 0);
    assert(0 == a->children[0]->value.constructor.length);
    
    destr(a);
    assert(a->length == 0);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% types\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_build_single_tree);
    test(test_build_complex_tree);
    
    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
