#ifndef CORE_LOGGING_H_
#define CORE_LOGGING_H_

#include <stdio.h>

#ifndef NDEBUG
#define CORE_LOGGING_ENABLED 1
#else
#define CORE_LOGGING_ENABLED 0
#endif

#ifndef LOGGING_NO_PREFIX
#define LOGGING_INTERNAL_PREFIX "%s:%d:%s(): "
#define LOGGING_INTERNAL_PREFIX_FMT __FILE__, __LINE__, __func__
#else
#define LOGGING_INTERNAL_PREFIX "%s"
#define LOGGING_INTERNAL_PREFIX_FMT ""
#endif

#define LOGGING_INTERNAL(level, lvlname, fmt, ...)                            \
    do                                                                        \
    {                                                                         \
        if ((level) <= core_logging_get_level())                              \
        {                                                                     \
            (void) fprintf(stderr,                                            \
                           "[" lvlname "] " LOGGING_INTERNAL_PREFIX fmt "\n", \
                           LOGGING_INTERNAL_PREFIX_FMT,                       \
                           __VA_ARGS__);                                      \
        }                                                                     \
    } while (0)

// log levels
typedef enum core_logging_level_t
{
    LOGLEVEL_FATAL = 0,
    LOGLEVEL_ERROR,
    LOGLEVEL_WARN,
    LOGLEVEL_INFO,
    LOGLEVEL_DEBUG,
} core_logging_level_t;

core_logging_level_t core_logging_get_level(void);
void core_logging_set_level(core_logging_level_t level);

// Macros intended to be used by the user

#define LOGGING_INIT(level) core_logging_set_level(level)

#define LOG_DEBUG(fmt, ...)                                              \
    do                                                                   \
    {                                                                    \
        if (CORE_LOGGING_ENABLED)                                   \
            LOGGING_INTERNAL(LOGLEVEL_DEBUG, "DEBUG", fmt, __VA_ARGS__); \
    } while (0)

#define LOG_INFO(fmt, ...) LOGGING_INTERNAL(LOGLEVEL_INFO, " INFO", fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) LOGGING_INTERNAL(LOGLEVEL_WARN, " WARN", fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOGGING_INTERNAL(LOGLEVEL_ERROR, "ERROR", fmt, __VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOGGING_INTERNAL(LOGLEVEL_FATAL, "FATAL", fmt, __VA_ARGS__)

#define LOG_DEBUG_MSG(msg) LOG_DEBUG("%s", (msg))
#define LOG_INFO_MSG(msg) LOG_INFO("%s", (msg))
#define LOG_WARN_MSG(msg) LOG_WARN("%s", (msg))
#define LOG_ERROR_MSG(msg) LOG_ERROR("%s", (msg))
#define LOG_FATAL_MSG(msg) LOG_FATAL("%s", (msg))

#endif /* ifndef CORE_LOGGING_H_ */
