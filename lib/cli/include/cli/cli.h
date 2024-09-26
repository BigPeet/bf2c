#ifndef CLI_CLI_H_
#define CLI_CLI_H_

#include <stddef.h>
#include "cli/option.h"

// cli_t: Struct and functions

typedef struct cli_t
{
    size_t count;
    char const* program_name;
    char const* program_description;
    struct cli_option_t* options;
} cli_t;

void cli_print_usage(cli_t const* cli);
bool cli_parse_args(cli_t const* cli, int argc, char** argv);


// User MACROS for setup and initialization

#define CLI_OPTION(name, short_form, param, type, default_val, desc)             \
    {                                                                            \
        .long_name = (name), .short_name = (short_form), .parameter = (param),   \
        .description = (desc), .default_value = {.type##_value = (default_val)}, \
        .value_type = (type), .given_value = {                                   \
            0                                                                    \
        }                                                                        \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define CLI_SETUP_OPTIONS(name, desc, ...)                                                              \
    static cli_t* cli_options_setup_global_gen(void)                                                    \
    {                                                                                                   \
        static cli_option_t options[] = {__VA_ARGS__};                                                  \
        static cli_t cli              = {.program_name        = (name),                                 \
                                         .program_description = (desc),                                 \
                                         .count               = sizeof(options) / sizeof(cli_option_t), \
                                         .options             = options};                                           \
        return &cli;                                                                                    \
    }

#define CLI_INIT_OPTIONS(var_name) cli_t const* var_name = cli_options_setup_global_gen()


#endif /* ifndef CLI_CLI_H_ */
