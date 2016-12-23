#include "log.h"

void trace(const char *message, ...){
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    printf("\n");
    va_end(args);
}