#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "types.h"


void test_build_single_tree() {
    AST *a = constr("A", 0);
    assert(a->length == 0);
    assert(a->children == NULL);

    destr(a);
    assert(a->symbol == NULL);
}

void test_build_complex_tree() {
    AST *a = constr("A", 2, constr("B", 0), constr("C", 0));
    assert(strncmp("B", a->children[0].symbol, 10) == 0);
    assert(strncmp("C", a->children[1].symbol, 10) == 0);
    assert(0 == a->children[0].length);
    
    destr(a);
    assert(a->length == 0);
    assert(a->children == NULL);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% types\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%%  test_build_single_tree (types)\n");
    test_build_single_tree();
    printf("%%TEST_FINISHED%% time=0 test_build_single_tree (types)\n");

    printf("%%TEST_STARTED%%  test_build_complex_tree (types)\n");
    test_build_complex_tree();
    printf("%%TEST_FINISHED%% time=0 test_build_complex_tree (types)\n");

    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
