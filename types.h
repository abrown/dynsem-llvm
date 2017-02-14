#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#ifndef TYPES_H
#define TYPES_H

typedef enum type_t {
    CONSTANT, VARIABLE, WILDCARD, CONSTRUCTOR, TUPLE
} Tag;

typedef struct term_t Term;

typedef char * Symbol;

typedef struct term_t {
    Tag tag;
    Term *ref;
    int symbol_length;
    Symbol symbol;
    int children_length;
    Term *children[];
} Term;

typedef struct rule_t {
    Term from;
    Term to;
} Rule;

typedef struct rules_t {
    int length;
    Rule *rules;
} Rules;

// need to take advantage of stack frames for environments and stores; when evaluating premises we may have to backtrack and frames (or a journal) will let us jump back

typedef struct map_t {
} Map;


/**
 * Helper function for creating a term
 * @param symbol
 * @param numChildren
 * @param ...
 * @return 
 */
Term *constr(Tag tag, Symbol symbol, int numChildren, ...);

/**
 * Helper function for destroying a term
 * @param constr
 * @return 
 */
int destr(Term *constr);

#endif /* TYPES_H */

