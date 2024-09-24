#ifndef LOGGING_LOGGING_H_
#define LOGGING_LOGGING_H_

#include <stdio.h>

#ifdef DEBUG
#define LOGGING_DEBUG 1
#else
#define LOGGING_DEBUG 0
#endif

#define LOGGING_INTERNAL(level, fmt, ...) \
    (void)                                \
        fprintf(stderr, "[" level "] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...)                              \
    do                                                   \
    {                                                    \
        if (LOGGING_DEBUG)                               \
            LOGGING_INTERNAL("DEBUG", fmt, __VA_ARGS__); \
    } while (0)
#define LOG_INFO(fmt, ...) LOGGING_INTERNAL(" INFO", fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) LOGGING_INTERNAL(" WARN", fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOGGING_INTERNAL("ERROR", fmt, __VA_ARGS__)

#endif /* ifndef LOGGING_LOGGING_H_ */
