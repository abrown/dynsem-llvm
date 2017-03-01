#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_assert.h"
#include "aterm1.h"
#include "aterm2.h"
#include "generate_aterms.h"

void learn_to_make_aterms() {
    ATerm a = ATmake("<int>", 42);
    assert(ATgetType(a) == AT_INT);
}

void learn_to_match_aterms() {
    int left, right;
    char *fun;
    ATbool result;

    ATerm a = ATmake("c(<int>, <int>)", 42, 53);
    result = ATmatch(a, "<appl(<int>, <int>)>", &fun, &left, &right);
    assert(result == ATtrue);
    assert(fun[0] == 'c');
    assert(fun[1] == 0);
    assert(left == 42);
    assert(right == 53);
}

/*
 * Demonstrate how find 0-arity functions and replace them with empty <term> (placeholder?)
 */
void learn_to_clone_and_replace() {
    ATerm rule_from = ATmake("a(x, y, z)");
    ATerm rule_to = ATmake("b(y)");
    ATerm before = ATmake("a(<int>, <int>, <int>)", 1, 2, 3);

    ATerm x = ATgetArgument(rule_from, 0);
    assert(ATgetType(x) == AT_APPL);
    assert(ATgetName(ATgetAFun(x))[0] == 'x');
    assert(ATgetArity(ATgetAFun(x)) == 0);

    ATerm x2, y, z;
    ATbool result;
    ATerm rule_from_patternized = replace_free_variables(rule_from);
    result = ATmatchTerm(before, rule_from_patternized, &x2, &y, &z);
    assert(result == ATtrue);
    assert(ATgetType(x2) == AT_INT);
    assert(ATgetInt(x2) == 1);

    ATerm rule_to_patternized = replace_free_variables(rule_to);
    ATerm after = ATmakeTerm(rule_to_patternized, y);
    assert(ATgetInt(ATgetArgument(after, 0)) == 2);
}

#define VAR_NAME_AT(l, i) ATgetName(ATgetAFun(ATelementAt(l, i)))

void test_find_free_variables() {
    ATerm a = ATmake("x(a, y(b, c, [d, e]))");
    ATermList vars = find_free_variables(a, ATempty);

    assert(ATgetLength(vars) == 5);
    assert(VAR_NAME_AT(vars, 0)[0] == 'a');
    assert(VAR_NAME_AT(vars, 1)[0] == 'b');
    assert(VAR_NAME_AT(vars, 2)[0] == 'c');
    assert(VAR_NAME_AT(vars, 3)[0] == 'd');
    assert(VAR_NAME_AT(vars, 4)[0] == 'e');
}

void test_replace_free_variables() {
    ATerm a = ATmake("a(b, c)");
    ATprintf("Before: %t\n", a);
    ATerm a_ = replace_free_variables(a);
    ATprintf("After: %t\n", a_);
    // assert(ATisEqual(a_, ATmake("a(<term>, <term>)")) == ATtrue); // TODO why doesn't this work?
}

void test_generate_variable_list() {
    char *buffer;
    size_t buffer_size;
    FILE* fd = open_memstream(&buffer, &buffer_size);
    ATermList vars = ATmakeList3(ATmake("a"), ATmake("b"), ATmake("c"));

    generate_variable_list(fd, vars, NULL, ", ", NULL);
    fputc(0, fd); // to end the list string
    assert(strncmp(buffer, "a, b, c", 1024) == 0);

    fclose(fd);
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    printf("%%SUITE_STARTING%% aterms\n");
    printf("%%SUITE_STARTED%%\n");

    test(learn_to_make_aterms);
    test(learn_to_match_aterms);
    test(learn_to_clone_and_replace);
    test(test_find_free_variables);
    test(test_replace_free_variables);
    test(test_generate_variable_list);

    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
