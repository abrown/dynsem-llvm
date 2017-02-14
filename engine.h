#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "log.h"

#ifndef ENGINE_H
#define ENGINE_H

int matches(const Term *instance, const Term *pattern);
void execute(Rules *rules, Term *ast);
void clone(const Term *from, Term *to);

#endif /* ENGINE_H */

