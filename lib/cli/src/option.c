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

static bool parse_int_from_str(int* value, char const* str)
{
    assert(value);
    assert(str);
    char* end                = NULL;
    errno                    = 0;
    const long val           = strtol(str, &end, 0); // auto-detect base
    const bool range_error   = (val == LONG_MIN || val == LONG_MAX) && errno == ERANGE;
    const bool no_conversion = val == 0 && str == end;
    *value                   = (int) val;
    return !range_error && !no_conversion;
}


static bool parse_double_from_str(double* value, char const* str)
{
    assert(value);
    assert(str);
    char* end                = NULL;
    errno                    = 0;
    const double val         = strtod(str, &end);
    const bool range_error   = val == HUGE_VAL && errno == ERANGE;
    const bool no_conversion = val == 0.0 && str == end;
    *value                   = val;
    return !range_error && !no_conversion;
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

static void cli_option_value_print(cli_option_value_type_t type, cli_option_value_t value)
{
    switch (type)
    {
        case STRING:
            printf("%s", value.STRING_value ? value.STRING_value : "");
            break;
        case INT:
            printf("%d", value.INT_value);
            break;
        case DOUBLE:
            printf("%f", value.DOUBLE_value);
            break;
        case BOOL:
            printf("%s", value.BOOL_value ? "true" : "false");
            break;
    }
}

static void cli_option_values_print(cli_option_value_type_t type,
                                    cli_option_value_t* values,
                                    size_t values_len)
{
    assert(values_len == 0 || values);
    for (size_t i = 0; i < values_len; ++i)
    {
        cli_option_value_print(type, values[i]);
        if (i < values_len - 1)
        {
            printf(", ");
        }
    }
}

static bool cli_option_value_init(cli_option_value_type_t type,
                                  char const* argument,
                                  cli_option_value_t* out)
{
    assert(argument);
    assert(out);
    bool success = false;
    switch (type)
    {
        case STRING:
            out->STRING_value = argument;
            success           = true;
            break;
        case INT:
            success = parse_int_from_str(&out->INT_value, argument);
            break;
        case DOUBLE:
            success = parse_double_from_str(&out->DOUBLE_value, argument);
            break;
        case BOOL:
            success = parse_bool_from_str(&out->BOOL_value, argument);
            break;
    }
    return success;
}

void cli_option_print_usage(cli_option_t const* option)
{
    ABORT_IF(!option);
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

void cli_option_print_value(cli_option_t const* option)
{
    ABORT_IF(!option);
    printf("\t- %s := ",
           option->long_name    ? option->long_name
           : option->short_name ? &option->short_name
                                : "");
    if (option->uses_multiple_values)
    {
        cli_option_values_print(option->value_type, option->contained.values, option->values_len);
    }
    else
    {
        cli_option_value_print(option->value_type, option->contained);
    }
    printf("\n");
}

void cli_option_destroy(cli_option_t* option)
{
    if (option && option->uses_multiple_values)
    {
        free(option->contained.values);
    }
}

bool cli_option_same_long_name(cli_option_t const* option, char const* name)
{
    ABORT_IF(!option || !name);
    return option->long_name && strcmp(option->long_name, name) == 0;
}

bool cli_option_same_short_name(cli_option_t const* option, char name)
{
    ABORT_IF(!option);
    return option->short_name && option->short_name == name;
}

cli_result_t cli_option_enable_flag(cli_option_t* option)
{
    ABORT_IF(!option);
    if (option->value_type != BOOL)
    {
        return CLI_ERR(CLI_ERROR_INVALID_OPTION_TYPE, option->long_name);
    }
    option->contained.BOOL_value = true;
    return CLI_OK();
}


cli_result_t cli_option_set_value(cli_option_t* option, char const* argument)
{
    ABORT_IF(!option || !argument);
    LOG_DEBUG("Setting value for option '%s' to '%s'.", option->long_name, argument);
    return cli_option_value_init(option->value_type, argument, &option->contained)
               ? CLI_OK()
               : CLI_ERR(CLI_ERROR_INVALID_OPTION_TYPE, argument);
}


cli_result_t cli_option_set_values(cli_option_t* option, size_t num_values, char** arguments)
{
    ABORT_IF(!option || !arguments || num_values < 2);

    cli_option_value_t* values = malloc(num_values * sizeof(cli_option_value_t));
    LOG_AND_ABORT_IF(values == NULL, "Allocation failed.");

    for (size_t i = 0; i < num_values; ++i)
    {
        LOG_DEBUG("Setting %zu. value for option '%s' to '%s'.",
                  i + 1,
                  option->long_name,
                  arguments[i]);
        if (!cli_option_value_init(option->value_type, arguments[i], &values[i]))
        {
            free(values);
            return CLI_ERR(CLI_ERROR_INVALID_OPTION_TYPE, arguments[i]);
        }
    }

    option->contained.values = values;
    option->values_len       = num_values;
    return CLI_OK();
}
