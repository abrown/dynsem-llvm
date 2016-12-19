#include <stdio.h>
#include "types.h"

void trace(char *message, void *any){
    printf(message);
    printf("\n");
}

int matches(const AST *instance, const AST *pattern) {
    
    return 0;
}

const Rule *find_match(const AST *ast, const Rules *rules){
    for(int i = 0; i < rules->length; i++) {
        if(matches(ast, &rules->rules[i].from)){
            return &rules->rules[i];
        }
    }
    return 0;
}

int pair_symbols(const Rule *match, const AST *ast, Map *symbols) {
    return 0;
}

void construct(const Rule *match, const Map *symbols, AST *constructed){
    
}

void execute(Rules *rules, AST *ast) {
    const Rule *match = find_match(ast, rules);
    if(match == 0) {
        trace("Failed AST match", ast);
    } else {
        Map symbols;
        pair_symbols(match, ast, &symbols);
        AST constructed;
        construct(match, &symbols, &constructed);
    }
}

