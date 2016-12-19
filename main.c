#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "engine.h"

int main(int argc, char** argv) {
    AST a1;
    AST a2;
    Rule a = {.from = a1, .to = a2};
    AST a3;
    
    Rules r;
    r.length = 1;
    r.rules = malloc(sizeof(Rule));
    
    
    execute(&r, &a3);
    return (EXIT_SUCCESS);
}

