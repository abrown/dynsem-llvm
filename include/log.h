#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef LOG_H
#define LOG_H

void trace(const char *message, ...);
void log_and_exit(const int code, const char *message, ...);

#endif /* LOG_H */

