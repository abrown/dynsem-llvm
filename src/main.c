#include <stdio.h>
#include <stdlib.h>
#include <aterm1.h>
#include <aterm2.h>
#include "types.h"
#include "generate.h"
#include "logging.h"

extern List_T dynsem_parse(FILE *fd);

RuleTable *convert(List_T spec) {
    int num_rules = List_length(spec);
    RuleTable *rules = malloc(sizeof (RuleTable) + num_rules * sizeof (Rule));
    rules->length = 3;

    for (int i = 0; spec; spec = spec->rest, i++) {
        Rule rule = *(Rule *) spec->first;
        rules->rules[i] = rule;

        rule.premises_length = List_length(rule.premise_list);
        rule.premises = NULL;
        if (rule.premises_length > 0) {
            rule.premises = malloc(rule.premises_length * sizeof (Premise));
            List_T old_premises = rule.premise_list;
            for (int i = 0; old_premises; old_premises = old_premises->rest, i++) {
                Premise premise = *(Premise *) old_premises->first;
                rule.premises[i] = premise;
            }
        }
    }
    
    return rules;
}

RuleTable *parse(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: dynsem-llvm spec.ds\n");
        exit(EXIT_FAILURE);
    }

    FILE* fd = fopen(argv[1], "r");
    List_T spec = dynsem_parse(fd);
    fclose(fd);
    log_info("Found %d rules", List_length(spec));

    return convert(spec);
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

