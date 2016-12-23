#include <stdarg.h>
#include <stdlib.h>
#include "log.h"

#ifndef TYPES_H
#define TYPES_H

typedef enum type_t {
    VARIABLE, CONSTANT, WILDCARD, CONSTRUCTOR, TUPLE
} Type;
typedef char * Symbol;

typedef struct ast_t {
    Symbol symbol;
    int length;
    struct ast_t **children;
} AST;

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


AST *constr(Symbol symbol, int numChildren, ...);
int destr(AST *ast);

#endif /* TYPES_H */

