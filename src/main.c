#include <stdio.h>
#include <stdlib.h>
#include <aterm1.h>
#include <aterm2.h>
#include "types.h"
#include "generate.h"

RuleTable *parse(int argc, char** argv) {
    Premise *premises = malloc(sizeof (Premise));
    premises[0] = (Premise){.type = EQUALITY, .left = ATmake("x"), .right = ATmake("0")};
    
    RuleTable *rules = malloc(sizeof (RuleTable) + 3 * sizeof (Rule));
    rules->length = 3;
    rules->rules[0] = (Rule){.from = ATmake("a(x, y)"), .to = ATmake("b(x)"), .premises_length = 1, .premises = premises};
    rules->rules[1] = (Rule){.from = ATmake("b(x)"), .to = ATmake("c(x, 0, 42)"), .premises_length = 0, .premises = NULL};
    rules->rules[2] = (Rule){.from = ATmake("c(x, y, z)"), .to = ATmake("z"), .premises_length = 0, .premises = NULL};
    return rules;
}

RuleTable *parse2(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: dynsem-llvm spec.ds\n");
        exit(EXIT_FAILURE);
    }

    RuleTable *rules;
    FILE* fd = fopen(argv[1], "r");
    // TODO 
    fclose(fd);
    return rules;
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    RuleTable *rules = parse(argc, argv);

    FILE* fd = fopen("generated/transform.c", "w");
    generate_headers(fd);
    generate_rule_table(fd, rules);
    generate_transform_functions(fd, rules);
    generate_find_function(fd, rules);
    fclose(fd);

    free(rules);
    return EXIT_SUCCESS;
}

