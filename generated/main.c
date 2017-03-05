#include <stdio.h>
#include <stdlib.h>
#include <aterm1.h>
#include <aterm2.h>
#include "transform.h"

ATerm parse(int argc, char **argv) {
    if(argc < 2){ 
        printf("Usage: interpreter file\n");
        exit(EXIT_FAILURE);
    }
    return ATreadFromNamedFile(argv[1]);
}

ATbool is_finished(ATerm term) {
    return term == NULL;
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    build_rules();
    ATerm term = parse(argc, argv);

    int steps = 0;
    while (!is_finished(term)) {
        ATprintf("#%d %t\n", steps++, term);
        term = match_and_transform(term);
    }

    return EXIT_SUCCESS;
}

