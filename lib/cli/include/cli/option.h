#ifndef CLI_OPTION_H_
#define CLI_OPTION_H_

#include <stdbool.h>
#include "cli/result.h"

// forward declaration
union cli_option_value_t;

typedef union cli_option_value_t
{
    double DOUBLE_value;
    char const* STRING_value;
    int INT_value;
    bool BOOL_value;

    // Alternatively, we could store arrays of the specific types.
    // That COULD be more efficient, but would make the API more complex.
    union cli_option_value_t* values;
} cli_option_value_t;


typedef enum cli_option_value_type_t
{
    STRING,
    INT,
    DOUBLE,
    BOOL,
} cli_option_value_type_t;


typedef struct cli_option_t
{
    cli_option_value_t contained;
    size_t values_len;
    char const* long_name;
    char const* parameter;
    char const* description;
    cli_option_value_type_t value_type;
    char short_name;
    bool uses_multiple_values;
} cli_option_t;

void cli_option_print_usage(cli_option_t const* option);
void cli_option_print_value(cli_option_t const* option);
void cli_option_destroy(cli_option_t* option);
bool cli_option_same_long_name(cli_option_t const* option, char const* name);
bool cli_option_same_short_name(cli_option_t const* option, char name);
cli_result_t cli_option_enable_flag(cli_option_t* option);
cli_result_t cli_option_set_value(cli_option_t* option, char const* argument);
cli_result_t cli_option_set_values(cli_option_t* option, size_t num_values, char** arguments);

// MACROs for convenience
#define CLI_OPTION(name, short_form, param, type, default_val, desc)                        \
    {                                                                                       \
        .contained = {.type##_value = (default_val)}, .values_len = 0, .long_name = (name), \
        .parameter = (param), .description = (desc), .value_type = (type),                  \
        .short_name = (short_form), .uses_multiple_values = false,                          \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define CLI_MULTI_OPTION(name, short_form, param, type, desc)                                      \
    {                                                                                              \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .parameter = (param), \
        .description = (desc), .value_type = (type), .short_name = (short_form),                   \
        .uses_multiple_values = true,                                                              \
    }

#define COMMON_OPTIONS()                                                                    \
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"), \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                              \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#endif /* ifndef CLI_OPTION_H_ */
