#include <stdio.h>
#include <stdlib.h>
#include <aterm2.h>
#include "types.h"
#include "generate.h"

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    RuleTable *rules = malloc(sizeof (RuleTable) + 3 * sizeof (Rule));
    rules->length = 3;
    rules->rules[0] = (Rule){.id = 0, .from = ATmake("a(x, y)"), .to = ATmake("b(x)")};
    rules->rules[1] = (Rule){.id = 1, .from = ATmake("b(x)"), .to = ATmake("c(x, 0, 42)")};
    rules->rules[2] = (Rule){.id = 2, .from = ATmake("c(x, y, z)"), .to = ATmake("z")};

    FILE* fd = fopen("generated/transform.c", "w");
    generate_headers(fd);
    generate_rule_table(fd, rules);
    generate_find_function(fd, rules);
    fclose(fd);

    free(rules);
    return EXIT_SUCCESS;
}

