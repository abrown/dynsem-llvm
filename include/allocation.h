#include <cii/list.h>
#include "types.h"

#ifndef ALLOCATION_H
#define ALLOCATION_H

#ifdef __cplusplus
extern "C" {
#endif

    Rule *rule_allocate(char *from, char *to, List_T premises);
    Native *native_allocate(char *name, char *code);
    Premise *premise_allocate(char *left, char *right, PremiseType type);

#ifdef __cplusplus
}
#endif

#endif /* ALLOCATION_H */

