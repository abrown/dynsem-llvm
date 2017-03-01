#include <aterm1.h> 

#ifndef TYPES_H
#define TYPES_H

typedef struct rule_t {
    int id;
    ATerm from;
    ATerm to;
} Rule;

typedef struct rule_table_t {
    int length;
    Rule rules[];
} RuleTable;

typedef enum return_code_t {
    SUCCESS = 0, FAILURE
} Result;

#endif /* TYPES_H */

