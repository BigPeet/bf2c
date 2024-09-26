#ifndef CORE_ABORT_H_
#define CORE_ABORT_H_

#include <stdlib.h>
#include "core/logging.h"

#define ABORT() abort()

#define ABORT_IF(cond)                                 \
    do                                                 \
    {                                                  \
        if ((cond))                                    \
        {                                              \
            LOG_FATAL_MSG("Condition failed: " #cond); \
            ABORT();                                   \
        }                                              \
    } while (0)

#define LOG_AND_ABORT(msg)    \
    do                        \
    {                         \
        LOG_FATAL_MSG((msg)); \
        ABORT();              \
    } while (0)

#define LOG_AND_ABORT_IF(cond, msg) \
    do                              \
    {                               \
        if ((cond))                 \
        {                           \
            LOG_AND_ABORT((msg));   \
        }                           \
    } while (0)

#endif /* ifndef CORE_ABORT_H_ */
