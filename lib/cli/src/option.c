#include "cli/option.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli/error_codes.h"
#include "cli/result.h"
#include "core/abort.h"
#include "core/logging.h"


void cli_option_print(cli_option_t const* option)
{
    assert(option);
    if (option->short_name)
    {
        printf("-%c, ", option->short_name);
    }
    else
    {
        printf("    ");
    }
    printf("--%s", option->long_name);
    if (option->parameter)
    {
        printf(" <%s>", option->parameter);
    }
    printf(" %s\n", option->description);
}

bool cli_option_same_long_name(cli_option_t const* option, char const* name)
{
    assert(option);
    assert(name);
    return option->long_name && strcmp(option->long_name, name) == 0;
}

bool cli_option_same_short_name(cli_option_t const* option, char const* name)
{
    assert(option);
    assert(name);
    return option->short_name && option->short_name == *name;
}

cli_result_t cli_option_enable_flag(cli_option_t* option)
{
    ABORT_IF(!option);
    if (option->value_type != BOOL)
    {
        return cli_result_t_create_from_error(CLI_ERROR_INVALID_OPTION_TYPE);
    }
    option->given_value.BOOL_value = true;
    return cli_result_t_create_from_value();
}

static bool parse_int_from_str(int* value, char const* str)
{
    assert(value);
    assert(str);
    errno                  = 0;
    const long val         = strtol(str, NULL, 0); // auto-detect base
    const bool range_error = (val == LONG_MIN || val == LONG_MAX) && errno == ERANGE;
    *value                 = (int) val;
    return !range_error;
}


static bool parse_double_from_str(double* value, char const* str)
{
    assert(value);
    assert(str);
    errno                  = 0;
    const double val       = strtod(str, NULL); // auto-detect base
    const bool range_error = val == HUGE_VAL && errno == ERANGE;
    *value                 = val;
    return !range_error;
}


static bool parse_bool_from_str(bool* value, char const* str)
{
    assert(value);
    assert(str);
    if (strcmp(str, "true") == 0 || strcmp(str, "1") == 0)
    {
        *value = true;
        return true;
    }
    if (strcmp(str, "false") == 0 || strcmp(str, "0") == 0)
    {
        *value = false;
        return true;
    }
    return false;
}


// TODO: return result...cli_parse_result_t? same error codes as cli_parse_result_t?
//       Add common place for some common result definitions.
cli_result_t cli_option_set_value(cli_option_t* option, char const* parameter)
{
    ABORT_IF(!option || !parameter);
    LOG_DEBUG("Setting value for option '%s' to '%s'\n", option->long_name, parameter);
    switch (option->value_type)
    {
        case STRING:
            option->given_value.STRING_value = parameter;
            break;
        case INT:
            // try_unwrap
            if (!parse_int_from_str(&option->given_value.INT_value, parameter))
            {
                return cli_result_t_create_from_error(CLI_ERROR_INVALID_OPTION_TYPE);
            }
            break;
        case DOUBLE:
            if (!parse_double_from_str(&option->given_value.DOUBLE_value, parameter))
            {
                return cli_result_t_create_from_error(CLI_ERROR_INVALID_OPTION_TYPE);
            }
            break;
        case BOOL:
            if (!parse_bool_from_str(&option->given_value.BOOL_value, parameter))
            {
                return cli_result_t_create_from_error(CLI_ERROR_INVALID_OPTION_TYPE);
            }
            break;
    }
    return cli_result_t_create_from_value();
}
