#include "cli/error_codes.h"

#ifndef LOGGING_NO_PREFIX
#define LOGGING_NO_PREFIX 1
#define CLI_ERROR_CODES_CUSTOMIZED_PREFIX 1
#endif
#include "core/logging.h"
#ifdef CLI_ERROR_CODES_CUSTOMIZED_PREFIX
#undef LOGGING_NO_PREFIX
#undef CLI_ERROR_CODES_CUSTOMIZED_PREFIX
#endif

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
