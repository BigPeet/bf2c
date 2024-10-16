#ifndef CLI_OPTION_H_
#define CLI_OPTION_H_

#include <stdbool.h>
#include "cli/result.h"

typedef union cli_option_value_t
{
    double DOUBLE_value;
    char const* STRING_value;
    int INT_value;
    bool BOOL_value;
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
    char const* long_name;
    char const* parameter;
    char const* description;
    cli_option_value_t given_value;
    cli_option_value_type_t value_type;
    char short_name;
} cli_option_t;

void cli_option_print_usage(cli_option_t const* option);
void cli_option_print_value(cli_option_t const* option);
bool cli_option_same_long_name(cli_option_t const* option, char const* name);
bool cli_option_same_short_name(cli_option_t const* option, char name);
cli_result_t cli_option_enable_flag(cli_option_t* option);
cli_result_t cli_option_set_value(cli_option_t* option, char const* parameter);

// MACROs for convenience
#define CLI_OPTION(name, short_form, param, type, default_val, desc)           \
    {                                                                          \
        .long_name = (name), .short_name = (short_form), .parameter = (param), \
        .description = (desc), .given_value = {.type##_value = (default_val)}, \
        .value_type = (type),                                                  \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define COMMON_OPTIONS()                                                                    \
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"), \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                              \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#endif /* ifndef CLI_OPTION_H_ */
