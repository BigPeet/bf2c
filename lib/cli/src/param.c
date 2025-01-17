#include "cli/param.h"

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

static void cli_param_value_print(cli_param_value_type_t type, cli_param_value_t value)
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

static void cli_param_values_print(cli_param_value_type_t type,
                                   cli_param_value_t* values,
                                   size_t values_len)
{
    assert(values_len == 0 || values);
    for (size_t i = 0; i < values_len; ++i)
    {
        cli_param_value_print(type, values[i]);
        if (i < values_len - 1)
        {
            printf(", ");
        }
    }
}

static bool cli_param_value_init(cli_param_value_type_t type,
                                 char const* argument,
                                 cli_param_value_t* out)
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

void cli_param_print_usage(cli_param_t const* param)
{
    ABORT_IF(!param);
    if (param->is_positional)
    {
        printf("<%s%s>", param->long_name, param->may_use_multiple_values ? "..." : "");
    }
    else
    {
        if (param->short_name)
        {
            printf("-%c, ", param->short_name);
        }
        else
        {
            printf("    ");
        }
        printf("--%s", param->long_name);
        if (param->arg_name)
        {
            printf(" <%s>", param->arg_name);
        }
    }
    printf(" %s\n", param->description);
}

void cli_param_print_value(cli_param_t const* param)
{
    ABORT_IF(!param);
    printf("\t- %s := ",
           param->long_name    ? param->long_name
           : param->short_name ? &param->short_name
                               : "");
    if (param->uses_multiple_values)
    {
        cli_param_values_print(param->value_type, param->contained.values, param->values_len);
    }
    else
    {
        cli_param_value_print(param->value_type, param->contained);
    }
    printf("\n");
}


static void cli_param_clear_values(cli_param_t* param)
{
    assert(param);
    param->is_set_by_user = false;
    if (param->uses_multiple_values)
    {
        free(param->contained.values);
        param->contained.values = NULL;
        param->values_len       = 0;
    }
}

void cli_param_destroy(cli_param_t* param)
{
    if (param)
    {
        cli_param_clear_values(param);
    }
}


bool cli_param_same_long_name(cli_param_t const* param, char const* name)
{
    ABORT_IF(!param || !name);
    return param->long_name && strcmp(param->long_name, name) == 0;
}

bool cli_param_same_short_name(cli_param_t const* param, char name)
{
    ABORT_IF(!param);
    return param->short_name && param->short_name == name;
}

cli_result_t cli_param_enable_flag(cli_param_t* param)
{
    ABORT_IF(!param);
    if (param->value_type != BOOL)
    {
        return CLI_ERR(CLI_ERROR_INVALID_PARAMETER_TYPE, param->long_name);
    }
    param->contained.BOOL_value = true;
    return CLI_OK();
}


cli_result_t cli_param_set_value(cli_param_t* param, char const* argument)
{
    ABORT_IF(!param || !argument);
    LOG_DEBUG("Setting value for argument '%s' to '%s'.", param->long_name, argument);
    cli_param_clear_values(param);
    param->uses_multiple_values = false;
    param->is_set_by_user       = true;
    return cli_param_value_init(param->value_type, argument, &param->contained)
               ? CLI_OK()
               : CLI_ERR(CLI_ERROR_INVALID_PARAMETER_TYPE, argument);
}


cli_result_t cli_param_set_values(cli_param_t* param, size_t num_values, char** arguments)
{
    ABORT_IF(!param || !arguments || num_values < 2);

    cli_param_value_t* values = malloc(num_values * sizeof(cli_param_value_t));
    LOG_AND_ABORT_IF(values == NULL, "Allocation failed.");

    for (size_t i = 0; i < num_values; ++i)
    {
        LOG_DEBUG("Setting %zu. value for argument '%s' to '%s'.",
                  i + 1,
                  param->long_name,
                  arguments[i]);
        if (!cli_param_value_init(param->value_type, arguments[i], &values[i]))
        {
            free(values);
            return CLI_ERR(CLI_ERROR_INVALID_PARAMETER_TYPE, arguments[i]);
        }
    }

    cli_param_clear_values(param);
    param->contained.values     = values;
    param->values_len           = num_values;
    param->uses_multiple_values = true;
    param->is_set_by_user       = true;
    return CLI_OK();
}
