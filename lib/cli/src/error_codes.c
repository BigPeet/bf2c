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
    char* msg = "Unkown error"; // NOLINT
    switch (error->error_code)
    {
        case CLI_ERROR:
            msg = "Unspecified error occurred";
            break;
        case CLI_ERROR_MISSING_ARGUMENT:
            msg = "Parameter is missing an argument";
            break;
        case CLI_ERROR_INVALID_ARGUMENT:
            msg = "Parameter was given an invalid argument";
            break;
        case CLI_ERROR_UNKNOWN_PARAMETER:
            msg = "Unknown parameter encountered";
            break;
        case CLI_ERROR_INVALID_PARAMETER_TYPE:
            msg = "Parameter was given or accessed with an invalid type";
            break;
        case CLI_ERROR_INVALID_PARAMETER_SEQUENCE:
            msg = "Parameter in an invalid sequence";
            break;
        case CLI_ERROR_DUPLICATE_PARAMETER:
            msg = "Duplicate parameter encountered";
            break;
    }
    LOG_ERROR("%s: %s", msg, error->extra_msg);
}
