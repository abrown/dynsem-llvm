#include <aterm1.h>
#include <cii/list.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum { REDUCTION = 0, EQUALITY, INEQUALITY } PremiseType;

typedef struct premise_t {
    PremiseType type;
    ATerm left;
    ATerm right;
    int index; // used to identify the index of this premise in a generated table; -1 until generated, thereon >= 0
} Premise;

typedef struct rule_t {
    ATerm from;
    ATerm to;
    List_T premises; // unused in interpreter
    int index; // used to identify the index of this rule in a generated table; -1 until generated, thereon >= 0
} Rule;

typedef struct native_t {
    char *name;
    char *code;
} Native;

typedef struct spec_t {
    List_T rules;
    List_T natives;
} Specification;

typedef struct rule_table_t {
    int length;
    Rule rules[];
} RuleTable;

typedef enum return_code_t {
    SUCCESS = 0, FAILURE
} Result;

#endif /* TYPES_H */

