#include "cli/option.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/util/logging.h"


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

bool cli_option_has_long_name(cli_option_t const* option, char const* name)
{
    assert(option);
    assert(name);
    return option->long_name && strcmp(option->long_name, name) == 0;
}

bool cli_option_has_short_name(cli_option_t const* option, char name)
{
    assert(option);
    assert(name);
    return option->short_name && option->short_name == name;
}

void cli_option_enable_flag(cli_option_t* option)
{
    assert(option->value_type == BOOL);
    // set the tag anyway...to avoid potential issues down the line
    option->value_type             = BOOL;
    option->given_value.BOOL_value = true;
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


bool cli_option_set_value(cli_option_t* option, char const* parameter)
{
    assert(option);
    assert(parameter);

    LOG_DEBUG("Setting value for option '%s' to '%s'\n", option->long_name, parameter);
    switch (option->value_type)
    {
        case STRING:
            option->given_value.STRING_value = parameter;
            return true;
        case INT:
            return parse_int_from_str(&option->given_value.INT_value, parameter);
        case DOUBLE:
            return parse_double_from_str(&option->given_value.DOUBLE_value, parameter);
        case BOOL:
            return parse_bool_from_str(&option->given_value.BOOL_value, parameter);
    }
    return false;
}
