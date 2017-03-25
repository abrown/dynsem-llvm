#include <stdio.h>
#include <stdlib.h>
#include <aterm1.h>
#include <aterm2.h>
#include "types.h"
#include "generate.h"
#include "logging.h"
#include "parsing.h"

Specification *parse(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: dynsem-llvm spec.ds\n");
        exit(EXIT_FAILURE);
    }

    FILE* fd = fopen(argv[1], "r");
    Specification *spec = dynsem_parse(fd);
    fclose(fd);

    return spec;
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    Specification *spec = parse(argc, argv);

    FILE* fd = fopen("generated/transform.c", "w");
    assign_indices(spec);
    generate_headers(fd);
    generate_rule_table(fd, spec);
    generate_native_functions(fd, spec);
    generate_rule_functions(fd, spec);
    generate_match_function(fd, spec);
    fclose(fd);

    free(spec);
    return EXIT_SUCCESS;
}

