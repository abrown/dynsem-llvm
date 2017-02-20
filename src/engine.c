#include "engine.h"

// TODO substitute constructor strings for ints (e.g.) for faster comparison

int symbol_matches(const Symbol a, const Symbol b) {
    trace("symbol_matches: %s ?= %s", a, b);
    return strncmp(a, b, 1024);
}

/**
 * Match an instance of a term with a term pattern; TODO hash-consing (see aterms)
 * @param instance a concrete term (no wildcards, variables)
 * @param pattern a term that may contain wildcards, variables
 * @return 1 if the items match, 0 otherwise
 */
int matches(const Term *instance, const Term *pattern) {
    trace("matches");

    if (pattern->tag == WILDCARD || pattern->tag == VARIABLE) return 1;
    if (pattern->tag != instance->tag) return 0;

    if (pattern->symbol != instance->symbol) return 0;
    assert(pattern->symbol_length == instance->symbol_length);
    if (instance->tag == CONSTANT) return 1;

    if (pattern->children_length != instance->children_length) return 0;
    for (int i = 0; i < pattern->children_length; i++) {
        if (!matches(instance->children[i], pattern->children[i])) return 0;
    }

    return 1;
}

const Rule *find_match(const Term *ast, const Rules *rules) {
    trace("find match");
    for (int i = 0; i < rules->length; i++) {
        if (matches(ast, &rules->rules[i].from)) {
            trace("found match: %02x", &rules->rules[i]);
            return &rules->rules[i];
        }
    }
    return 0;
}

int pair_symbols(const Rule *match, const Term *ast, Map *symbols) {
    return 0;
}

/**
 * Clone an AST into a separate memory location (i.e. to); assumes that 
 * constants, variables, and wildcards are immutable and will not be freed
 * during the execution of the program.
 * @param from the address to clone from
 * @param to the address to clone to
 */
void clone(const Term *from, Term *to) {
    trace("clone %d", from->tag);
    memcpy(to, from, sizeof (Term) + from->children_length * sizeof (Term*));
    switch (from->tag) {
        case CONSTANT:
        case VARIABLE:
        case WILDCARD:
            // TODO do replacement here
            break;
        case CONSTRUCTOR:
        case TUPLE:
            for (int i = 0; i < from->children_length; i++) {
                to->children[i] = malloc(term_sizeof(from->children[i]));
                clone(from->children[i], to->children[i]);
            }
            break;
        default:
            log_and_exit(1, "clone: AST of unknown type: %d", from->tag);
            break;
    }
}

// attempt to rewrite ast using rules; if no match is found, rewrite children recursively and retry
// if we don't do the double match above then we are limited to applicative-order evaluation (vs normal order evaluation, p.567)

void execute(Rules *rules, Term *ast) {
    trace("execute");
    const Rule *match = find_match(ast, rules);
    if (match == 0) {
        trace("Failed AST match", ast);
    } else {
        Map symbols;
        pair_symbols(match, ast, &symbols);
        Term constructed;
        // construct(match, &symbols, &constructed);
    }
}