#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "types.h"
#include "log.h"

#ifndef ENGINE_H
#define ENGINE_H

int matches(const AST *instance, const AST *pattern);
void execute(Rules *rules, AST *ast);
void clone(const AST *from, AST *to);

#endif /* ENGINE_H */

