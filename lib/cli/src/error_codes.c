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
#include "core/abort.h"

void cli_print_error(cli_error_t const* error)
{
    ABORT_IF(!error);
    char* msg = "Unkown error";
    switch (error->error_code)
    {
        case CLI_ERROR:
            msg = "Unspecified error occurred";
            break;
        case CLI_ERROR_MISSING_PARAMETER:
            msg = "Option is missing a parameter";
            break;
        case CLI_ERROR_INVALID_PARAMETER:
            msg = "Option was given an invalid parameter";
            break;
        case CLI_ERROR_UNKNOWN_OPTION:
            msg = "Unknown option encountered";
            break;
        case CLI_ERROR_INVALID_OPTION_TYPE:
            msg = "Option was given or accessed with an invalid type";
            break;
    }
    LOG_ERROR("%s: %s", msg, error->extra_msg);
}
