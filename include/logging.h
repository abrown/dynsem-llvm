#include <stdio.h>

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#define log_error(format, ...) ATfprintf(stderr, "[ERROR] " format "\n", ##__VA_ARGS__);

#ifdef LOG_INFO
#define log_info(format, ...) ATfprintf(stderr, "[INFO] " format "\n", ##__VA_ARGS__);
#else
#define log_info(format, ...) {}
#endif

#ifdef LOG_DEBUG
#define log_debug(format, ...) ATfprintf(stderr, "[DEBUG] " format "\n", ##__VA_ARGS__);
#else
#define log_debug(format, ...) {}
#endif

#ifdef LOG_TRACE
#define log_trace(format, ...) ATfprintf(stderr, "[TRACE] " format "\n", ##__VA_ARGS__);
#else
#define log_trace(format, ...) {}
#endif

#ifdef LOG_CYCLE
#define log_cycle(format, ...) ATfprintf(stderr, "[CYCLE] " format "\n", ##__VA_ARGS__);
#else
#define log_cycle(format, ...) {}
#endif

#ifdef __cplusplus
}
#endif

#endif