#include <stdio.h>
#include "types.h"
#include "engine.h"

int main(int argc, char** argv) {
    int a[10];
    int b = 3223;
    int *c = &b;
    a[0] = *c;
    
    printf("a[0]: %d\n", a[0]);
    return 0;
//
//    AST a1;
//    AST a2;
//    Rule a = {.from = a1, .to = a2};
//    AST a3;
//
//
//
//    Rules r;
//    r.length = 1;
//    r.rules = malloc(sizeof (Rule));
//
//
//    execute(&r, &a3);
    return (EXIT_SUCCESS);
}

