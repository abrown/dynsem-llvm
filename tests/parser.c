#include <stdio.h>
#include <stdlib.h>
#include "test_assert.h"
int dynsem_parse(FILE *fd);

void test_parse_spec() {
    FILE *fd = fopen("specs/test.ds", "r");
    assert(dynsem_parse(fd) == 42);
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% parser\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_parse_spec);

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
