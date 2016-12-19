#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ast_t {
    char *symbol;
    int length;
    struct ast_t *chilren;
} AST;

typedef struct rule_t {
    AST from;
    AST to;
} Rule;

typedef struct rules_t {
    int length;
    Rule *rules;
} Rules;

typedef struct map_t {
    
} Map;


#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */

