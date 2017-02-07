#include <stdarg.h>
#include <stdlib.h>
#include "log.h"

#ifndef TYPES_H
#define TYPES_H

typedef enum type_t {
    CONSTANT, VARIABLE, WILDCARD, CONSTRUCTOR, TUPLE
} Type;

typedef struct ast_t AST;

typedef char * Symbol;

typedef struct constant_t {
    int length;
    char* value;
} Constant;

typedef struct variable_t {
    Symbol symbol;
} Variable;

typedef struct wildcard_t {
} Wildcard;

typedef struct constructor_t {
    Symbol symbol;
    int length;
    AST **children;
} Constructor;

typedef struct tuple_t {
    int length;
    AST **children;
} Tuple;

typedef struct ast_t {
    Type type;

    union {
        Constant constant;
        Variable variable;
        Wildcard wildcard;
        Constructor constructor;
        Tuple tuple;
    } value;
} AST;

typedef struct pattern_t {
    Type type;

    union {
        Constant constant;
        Variable variable;
        Wildcard wildcard;
        Constructor constructor;
        Tuple tuple;
    } value;
} Pattern;

typedef struct term_t {
    Type type;

    union {
        Constant constant;
        Constructor constructor;
        Tuple tuple;
    } value;
} Term;

typedef struct rule_t {
    AST from;
    AST to;
} Rule;

typedef struct rules_t {
    int length;
    Rule *rules;
} Rules;

// need to take advantage of stack frames for environments and stores; when evaluating premises we may have to backtrack and frames (or a journal) will let us jump back

typedef struct map_t {
} Map;


Constructor *constr(Symbol symbol, int numChildren, ...);
int destr(Constructor *constr);

#endif /* TYPES_H */

