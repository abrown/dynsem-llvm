#include "types.h"

Constructor *constr(Symbol symbol, int numChildren, ...) {
    trace("constr: %s, %d children", symbol, numChildren);

    Constructor *constr = malloc(sizeof (Constructor));
    constr->symbol = symbol;
    constr->length = numChildren;
    constr->children = NULL;

    if (numChildren > 0) {
        va_list arg;
        va_start(arg, numChildren);
        constr->children = malloc(numChildren * sizeof (AST*));
        for (int i = 0; i < numChildren; i++) {
            AST *c = va_arg(arg, AST*);
            constr->children[i] = c;
        }
        va_end(arg);
    }

    return constr;
}

int destr(Constructor *constr) {
    trace("destr: %s, %d children", constr->symbol, constr->length);

    // TODO need switch here
    if (constr->length > 0) {
        for (int i = 0; i < constr->length; i++) {
            // destr(constr->children[i]);
        }
    }

    constr->length = 0;
    constr->symbol = NULL;
    free(constr->children);
    free(constr);

    return 0;
}
