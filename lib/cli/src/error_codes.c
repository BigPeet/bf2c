#include "cli/error_codes.h"

#define LOGGING_NO_PREFIX 1
#include "core/logging.h"
#undef LOGGING_NO_PREFIX

void cli_print_error(cli_error_t error)
{
    switch (error)
    {
        case CLI_ERROR:
            LOG_ERROR_MSG("Unspecified error occurred!\n");
            break;
        case CLI_ERROR_MISSING_PARAMETER:
            LOG_ERROR_MSG("Option is missing a parameter!\n");
            break;
        case CLI_ERROR_INVALID_PARAMETER:
            LOG_ERROR_MSG("Option was given an invalid parameter!\n");
            break;
        case CLI_ERROR_UNKNOWN_OPTION:
            LOG_ERROR_MSG("Unknown option encountered!\n");
            break;
        case CLI_ERROR_INVALID_OPTION_TYPE:
            LOG_ERROR_MSG("Option was given or accessed with an invalid type!\n");
            break;
    }
}
