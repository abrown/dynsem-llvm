#include "types.h"

Term *constr(Tag tag, Symbol symbol, int numChildren, ...) {
    trace("constr: %s, %d children", symbol, numChildren);

    Term *created = malloc(sizeof (Term) + numChildren * sizeof(Term *));
    created->tag = tag;
    created->ref = NULL;
    created->symbol = symbol;
    created->symbol_length = symbol == NULL ? 0 : strnlen(symbol, 4096);
    
    created->children_length = numChildren;
    if (numChildren > 0) {
        va_list arg;
        va_start(arg, numChildren);
        for (int i = 0; i < numChildren; i++) {
            Term *c = va_arg(arg, Term*);
            created->children[i] = c;
        }
        va_end(arg);
    }

    return created;
}

int destr(Term *term) {
    trace("destr: %s, %d children", term->symbol, term->children_length);

    for (int i = 0; i < term->children_length; i++) {
        destr(term->children[i]);
    }

    memset(term, 0, sizeof (Term) + term->children_length * sizeof(Term *));
    free(term);

    return 0;
}
