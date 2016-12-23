#include "types.h"

AST *constr(Symbol symbol, int numChildren, ...) {
    trace("constr: %s, %d children", symbol, numChildren);
    AST *ast = malloc(sizeof (AST));
    ast->symbol = symbol;
    ast->length = numChildren;
    ast->children = NULL;

    if (numChildren > 0) {
        va_list arg;
        va_start(arg, numChildren);
        ast->children = malloc(numChildren * sizeof (AST*));
        for (int i = 0; i < numChildren; i++) {
            AST c = va_arg(arg, AST);
            ast->children[i] = c;
        }
        va_end(arg);
    }

    return ast;
}

int destr(AST *ast) {
    trace("destr: %s, %d children", ast->symbol, ast->length);

    if (ast->length > 0) {
        for (int i = 0; i < ast->length; i++) {
            destr(&ast->children[i]);
        }
    }

    ast->length = 0;
    ast->symbol = NULL;
    trace("AST: %02x", ast);
    //free(ast->children);
    //free(ast);

    return 0;
}
