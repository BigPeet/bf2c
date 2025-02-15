#include "core/logging.h"

static core_logging_level_t g_level = LOG_LEVEL_DEBUG;

core_logging_level_t core_logging_get_level(void)
{
    return g_level;
}

void core_logging_set_level(core_logging_level_t level)
{
    g_level = level;
}
