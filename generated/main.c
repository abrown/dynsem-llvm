#include <stdio.h>
#include <stdlib.h>
#include <aterm1.h>
#include <aterm2.h>
#include "transform.h"

ATerm parse() {
    return ATmake("a(1, 2)");
}

ATbool is_finished(ATerm term) {
    return ATfalse;
}

int main(int argc, char** argv) {
    ATerm bottom_of_stack;
    ATinit(argc, argv, &bottom_of_stack);

    build_rules();
    ATerm term = parse();

    int steps = 0;
    while (!is_finished(term)) {
        term = match_and_transform(term);
        ATprintf("#%d %t", steps++, term);
    }

    return EXIT_SUCCESS;
}

