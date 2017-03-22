#include <aterm1.h>
#include <cii/list.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum { REDUCTION = 0, EQUALITY, INEQUALITY } PremiseType;

typedef struct premise_t {
    PremiseType type;
    ATerm left;
    ATerm right;
} Premise;

typedef struct rule_t {
    ATerm from;
    ATerm to;
    List_T premise_list; // unused in interpreter
    int premises_length;
    Premise *premises;
} Rule;

typedef struct rule_table_t {
    int length;
    Rule rules[];
} RuleTable;

typedef enum return_code_t {
    SUCCESS = 0, FAILURE
} Result;

#endif /* TYPES_H */

