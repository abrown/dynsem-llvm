#include "engine.h"

// TODO substitute constructor strings for ints (e.g.) for faster comparison

int symbol_matches(const Symbol a, const Symbol b) {
    trace("symbol_matches: %s ?= %s", a, b);
    return strncmp(a, b, 1024);
}

int matches(const Term *instance, const Term *pattern) {
    trace("matches");
    if (pattern->tag == WILDCARD || pattern->tag == VARIABLE) return 1;
    if (pattern->tag != instance->tag) return 0;
    switch (instance->tag) {
        case CONSTRUCTOR:
            if (!symbol_matches(instance->value.constructor.symbol, pattern->value.constructor.symbol) == 0) return 0;
            if (pattern->value.constructor.length != instance->value.constructor.length) return 0;
            for (int i = 0; i < pattern->value.constructor.length; i++) {
                if (!matches(instance->value.constructor.children[i], pattern->value.constructor.children[i])) return 0;
            }
            break;
        case CONSTANT:
            if (instance->value.constant.length != pattern->value.constant.length) return 0;
            return !strncmp(instance->value.constant.value, pattern->value.constant.value, instance->value.constant.length); // TODO optimize
            break;
        case TUPLE:
            if (pattern->value.tuple.length != instance->value.tuple.length) return 0;
            for (int i = 0; i < pattern->value.tuple.length; i++) {
                if (!matches(instance->value.tuple.children[i], pattern->value.tuple.children[i])) return 0;
            }
            break;
        case VARIABLE:
        case WILDCARD:
        default:
            log_and_exit(1, "matches: AST of unknown type: %d", instance->tag);
            break;
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
    to->tag = from->tag;
    switch (from->tag) {
        case CONSTANT:
        case VARIABLE:
        case WILDCARD:
            to->value = from->value;
            break;
        case CONSTRUCTOR:
            to->value.constructor.symbol = from->value.constructor.symbol;
            to->value.constructor.length = from->value.constructor.length;
            Term** children = malloc(from->value.constructor.length * sizeof (Term*));
            for (int i = 0; i < from->value.constructor.length; i++) {
                children[i] = malloc(sizeof(Term));
                clone(from->value.constructor.children[i], children[i]);
            }
            to->value.constructor.children = children;
            break;
        case TUPLE:
            to->value.tuple.length = from->value.tuple.length;
            Term** tuple_children = malloc(from->value.tuple.length * sizeof (Term*));
            for (int i = 0; i < from->value.tuple.length; i++) {
                children[i] = malloc(sizeof(Term));
                clone(from->value.tuple.children[i], tuple_children[i]);
            }
            to->value.tuple.children = tuple_children;
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