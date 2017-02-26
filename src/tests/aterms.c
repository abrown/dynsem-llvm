#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_assert.h"
#include "aterm1.h"
#include "aterm2.h"

void make() {
    ATerm a = ATmake("<int>", 42);
    assert(ATgetType(a) == AT_INT);
}

void match() {
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

static AFun term = NULL;
static ATermPlaceholder term_placeholder = NULL;

ATerm replace_free_variables(ATerm rule) {
    switch (ATgetType(rule)) {
        case AT_APPL:
            trace("Found appl");
            AFun fun = ATgetAFun((ATermAppl) rule);
            int arity = ATgetArity(fun);
            if (arity == 0) {
                // generate and return "<term>"
                //if (term == NULL) {
                AFun term = ATmakeAFun("term", 0, ATfalse);
                ATermPlaceholder term_placeholder = ATmakePlaceholder((ATerm) ATmakeAppl0(term));
                //}
                //ATprintf("term_placeholder == %t\n", term_placeholder);
                return (ATerm) term_placeholder;
            } else {
                for (int i = 0; i < arity; i++) {
                    ATerm arg = ATgetArgument(rule, i);
                    ATerm new = replace_free_variables(arg);
                    if (arg != new) rule = (ATerm) ATsetArgument((ATermAppl) rule, new, i);
                }
            }
            break;
        case AT_LIST:
            trace("Found list");
            int length = ATgetLength((ATermList) rule);
            for (int i = 0; i < length; i++) {
                ATerm arg = ATelementAt((ATermList) rule, i);
                ATerm new = replace_free_variables(arg);
                if (arg != new) rule = (ATerm) ATreplace((ATermList) rule, new, i);
            }
            break;
        case AT_INT:
        case AT_REAL:
        case AT_BLOB:
        case AT_SYMBOL:
            return rule;
            break;
        case AT_PLACEHOLDER:
            ATwarning("Found placeholder in rule, continuing: %t", rule);
            return rule;
            break;
        case AT_FREE:
        default:
            ATerror("Found unknown or unhandleable type: %t", rule);
            exit(1);
            break;
    }
    return rule;
}

void test_replace_free_variables() {
    ATerm a = ATmake("a(b, c)");
    ATprintf("Before: %t\n", a);
    ATerm a_ = replace_free_variables(a);
    ATprintf("After: %t\n", a_);
    // assert(ATisEqual(a_, ATmake("a(<term>, <term>)")) == ATtrue); // TODO why doesn't this work?
}

// find 0-arity functions and replace them with empty <term> (placeholder?)

void clone_and_replace() {
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

static int level = 0;
static int num_vars = 0;

void generate_free_variable_allocation(FILE *stream, ATerm rule) {
    if (level == 0) {
        fputs("ATerm ", stream);
        num_vars = 0;
    }

    switch (ATgetType(rule)) {
        case AT_APPL:
            trace("Found appl");
            AFun fun = ATgetAFun((ATermAppl) rule);
            int arity = ATgetArity(fun);
            if (arity == 0) {
                char *name = ATgetName(ATgetAFun(rule));
                if (num_vars > 0) fputs(", ", stream);
                fputs(name, stream);
                num_vars++;
            } else {
                level++;
                for (int i = 0; i < arity; i++) {
                    ATerm arg = ATgetArgument(rule, i);
                    generate_free_variable_allocation(stream, arg);
                }
                level--;
            }
            break;
        case AT_LIST:
            trace("Found list");
            int length = ATgetLength((ATermList) rule);
            level++;
            for (int i = 0; i < length; i++) {
                ATerm arg = ATelementAt((ATermList) rule, i);
                generate_free_variable_allocation(stream, arg);
            }
            level--;
            break;
        case AT_INT:
        case AT_REAL:
        case AT_BLOB:
        case AT_SYMBOL:
        case AT_PLACEHOLDER:
        case AT_FREE:
        default:
            break;
    }

    if (level == 0) {
        fputs(";\n", stream);
        fflush(stream);
    }
}

void test_generate_free_variable_allocation() {
    char *buffer;
    size_t buffer_size;
    FILE* fd = open_memstream(&buffer, &buffer_size);
    ATerm a = ATmake("a(x, 1, z)");

    generate_free_variable_allocation(fd, a);

    assert(strncmp(buffer, "ATerm x, z;\n", 1024) == 0);
}

typedef struct rule_t {
    ATerm from;
    ATerm from_pattern;
    ATerm to;
} Rule;

typedef struct rule_table_t {
    int length;
    Rule rules[];
} RuleTable;

void generate_main(FILE *stream);
void generate_headers(FILE *stream);
void generate_rule_table(FILE *stream, RuleTable rules);
void generate_rule_table_entry(FILE *stream, Rule rule);
void generate_find_function(FILE *stream, RuleTable rules);
void generate_find_case(FILE *stream, Rule rule);
void generate_transform_functions(FILE *stream, RuleTable rules);
void generate_transform_function(FILE *stream, Rule rule);

//void stream_write_line(Stream stream, char *string);
//
//void generate_find_rule() {
//    int num_rules = 1;
//    Rule rule_table[num_rules];
//    rule_table[0]->from = ATmake("a(x, y, z)");
//    rule_table[0]->to = ATmake("b(y)");
//
//    stream_write_line(stream, "#include <aterm1.h>");
//    stream_write_line(stream, "#include <aterm2.h>");
//    stream_write_line(stream, "ATerm find(ATerm before) {");
//    for (int i = 0; i < num_rules; i++) {
//        stream_write_line(stream, "\t if(ATmatch(rule_table[" i "]->from_pattern, before)) return transform_" i "(rule_table[" i "], before);");
//        //        rule_table[i]->from_pattern = replace_free_variables(rule_table[i]->from);
//        //        write_free_variables(rule_table[0]->from);
//    }
//    stream_write_line(stream, "\t return error();");
//    stream_write_line(stream, "}");
//}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    printf("%%SUITE_STARTING%% aterms\n");
    printf("%%SUITE_STARTED%%\n");

    test(make);
    test(match);
    test(clone_and_replace);
    test(test_replace_free_variables);
    test(test_generate_free_variable_allocation);

    printf("%%SUITE_FINISHED%% time=0\n");
    return (EXIT_SUCCESS);
}
