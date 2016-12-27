#include "engine.h"

// TODO substitute constructor strings for ints (e.g.) for faster comparison

int symbol_matches(const Symbol a, const Symbol b) {
    trace("symbol_matches: %s ?= %s", a, b);
    return strncmp(a, b, 1024);
}

int matches(const AST *instance, const AST *pattern) {
    trace("matches");
    if (pattern->type == VARIABLE && pattern->type != instance->type) return 0;
    switch (instance->type) {
        case CONSTRUCTOR:
            if (!symbol_matches(instance->value.constructor.symbol, pattern->value.constructor.symbol) == 0) return 0;
            if (pattern->value.constructor.length != instance->value.constructor.length) return 0;
            for (int i = 0; i < pattern->value.constructor.length; i++) {
                if (!matches(instance->value.constructor.children[i], pattern->value.constructor.children[i])) return 0;
            }
            break;
        default:
            break;

    }
    return 1;
}

//

const Rule *find_match(const AST *ast, const Rules *rules) {
    trace("find match");
    for (int i = 0; i < rules->length; i++) {
        if (matches(ast, &rules->rules[i].from)) {
            trace("found match: %02x", &rules->rules[i]);
            return &rules->rules[i];
        }
    }
    return 0;
}

int pair_symbols(const Rule *match, const AST *ast, Map *symbols) {
    return 0;
}

// rebuild the ast with symbols replaced 
// we must clone the 'to' part of the rule, right?

void construct(const Rule *match, const Map *symbols, AST *constructed) {

}

// attempt to rewrite ast using rules; if no match is found, rewrite children recursively and retry
// if we don't do the double match above then we are limited to applicative-order evaluation (vs normal order evaluation, p.567)

void execute(Rules *rules, AST *ast) {
    trace("execute");
    const Rule *match = find_match(ast, rules);
    if (match == 0) {
        trace("Failed AST match", ast);
    } else {
        Map symbols;
        pair_symbols(match, ast, &symbols);
        AST constructed;
        construct(match, &symbols, &constructed);
    }
}