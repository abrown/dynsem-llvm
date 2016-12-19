#include "types.h"

#ifndef ENGINE_H
#define ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

void execute(Rules *rules, AST *ast);

#ifdef __cplusplus
}
#endif

#endif /* ENGINE_H */

