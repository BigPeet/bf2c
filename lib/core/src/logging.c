#include "core/logging.h"

static core_logging_level_t glevel = LOG_LEVEL_DEBUG;

core_logging_level_t core_logging_get_level(void)
{
    return glevel;
}

void core_logging_set_level(core_logging_level_t level)
{
    glevel = level;
}
