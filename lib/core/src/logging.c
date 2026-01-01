#include "core/logging.h"

// While this is a global variable, but
// - it has internal linkage (direct access is restricted to this file and through getter/setter),
// - it is a trivial type,
// - managing the log level through some local object would add a lot of complexity.
// So in this case, it is acceptable for me.
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static core_logging_level_t g_level = LOG_LEVEL_DEBUG;

core_logging_level_t core_logging_get_level(void) {
    return g_level;
}

void core_logging_set_level(core_logging_level_t level) {
    g_level = level;
}
