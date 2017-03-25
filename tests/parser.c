#include <stdio.h>
#include <stdlib.h>
#include <cii/list.h>
#include "test_assert.h"
#include "types.h"
#include "parsing.h"

void test_create_rules() {
    Rule a = {.from = ATmake("a(b, c)"), .to = ATmake("d(b)"), .premises = NULL};
    List_T rules = List_list(&a, NULL);
    assert(List_length(rules) == 1);

    Rule b = {.from = ATmake("a(b, c)"), .to = ATmake("d(b)")};
    rules = List_append(rules, List_list(&b, NULL));
    assert(List_length(rules) == 2);
}

void test_parse_spec() {
    FILE *fd = fopen("specs/example.ds", "r");
    Specification *spec = dynsem_parse(fd);
    assert(List_length(spec->rules) == 3);
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    printf("%%SUITE_STARTING%% parser\n");
    printf("%%SUITE_STARTED%%\n");

    test(test_create_rules);
    test(test_parse_spec);

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
