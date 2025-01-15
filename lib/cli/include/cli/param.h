#ifndef CLI_PARAM_H_
#define CLI_PARAM_H_

#include <stdbool.h>
#include "cli/result.h"

// forward declaration
union cli_param_value_t;

typedef union cli_param_value_t
{
    double DOUBLE_value;
    char const* STRING_value;
    int INT_value;
    bool BOOL_value;

    // Alternatively, we could store arrays of the specific types.
    // That COULD be more efficient, but would make the API more complex.
    union cli_param_value_t* values;
} cli_param_value_t;


typedef enum cli_param_value_type_t
{
    STRING,
    INT,
    DOUBLE,
    BOOL,
} cli_param_value_type_t;


typedef struct cli_param_t
{
    cli_param_value_t contained;
    size_t values_len;
    char const* long_name;
    char const* arg_name;
    char const* description;
    cli_param_value_type_t value_type;
    char short_name;
    bool uses_multiple_values;
    bool is_positional;
    bool is_set_by_user;
} cli_param_t;

void cli_param_print_usage(cli_param_t const* param);
void cli_param_print_value(cli_param_t const* param);
void cli_param_destroy(cli_param_t* param);
bool cli_param_same_long_name(cli_param_t const* param, char const* name);
bool cli_param_same_short_name(cli_param_t const* param, char name);
cli_result_t cli_param_enable_flag(cli_param_t* param);
cli_result_t cli_param_set_value(cli_param_t* param, char const* argument);
cli_result_t cli_param_set_values(cli_param_t* param, size_t num_values, char** arguments);

// MACROs for convenience
#define CLI_OPTION(name, short_form, param, type, default_val, desc)                        \
    {                                                                                       \
        .contained = {.type##_value = (default_val)}, .values_len = 0, .long_name = (name), \
        .arg_name = (param), .description = (desc), .value_type = (type),                   \
        .short_name = (short_form), .uses_multiple_values = false, .is_positional = false,  \
        .is_set_by_user = false,                                                            \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define CLI_MULTI_OPTION(name, short_form, param, type, desc)                                     \
    {                                                                                             \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = (param), \
        .description = (desc), .value_type = (type), .short_name = (short_form),                  \
        .uses_multiple_values = true, .is_positional = false, .is_set_by_user = false,            \
    }

#define CLI_POSITIONAL_ARG(name, type, default_val, desc)                                   \
    {                                                                                       \
        .contained = {.type##_value = (default_val)}, .values_len = 0, .long_name = (name), \
        .arg_name = NULL, .description = (desc), .value_type = (type), .short_name = '\0',  \
        .uses_multiple_values = false, .is_positional = true, .is_set_by_user = false,      \
    }

#define CLI_POSITIONAL_MULTI_ARG(name, type, desc)                                             \
    {                                                                                          \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = NULL, \
        .description = (desc), .value_type = (type), .short_name = '\0',                       \
        .uses_multiple_values = true, .is_positional = true, .is_set_by_user = false,          \
    }

#define COMMON_OPTIONS()                                                                        \
    CLI_FLAG("sep", 0, "\t\tNo-op to separate positional arguments."),                          \
        CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"), \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                                  \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#endif /* ifndef CLI_PARAM_H_ */
