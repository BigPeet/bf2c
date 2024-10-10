#ifndef CLI_CLI_H_
#define CLI_CLI_H_

#include <stddef.h>
#include "cli/option.h"
#include "cli/result.h"

// cli_t: Struct and functions

typedef struct cli_version_t
{
    int major;
    int minor;
    int patch;
} cli_version_t;

typedef struct cli_t
{
    cli_version_t version;
    size_t options_len;
    char const* program_name;
    char const* program_description;
    struct cli_option_t* options;
} cli_t;

void cli_print_usage(cli_t const* cli);
void cli_print_version(cli_t const* cli);
void cli_print_options(cli_t const* cli);
cli_result_t cli_parse_args(cli_t const* cli, int argc, char** argv);


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

#define COMMON_OPTIONS()                                                                    \
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"), \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                              \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#define CLI_SETUP(name, major, minor, patch, desc, ...)                                                 \
    static cli_t* cli_options_setup_global_gen(void)                                                    \
    {                                                                                                   \
        static cli_option_t options[] = {__VA_ARGS__};                                                  \
        static cli_t cli              = {.program_name        = (name),                                 \
                                         .program_description = (desc),                                 \
                                         .version             = {(major), (minor), (patch)},            \
                                         .options_len         = sizeof(options) / sizeof(cli_option_t), \
                                         .options             = options};                                           \
        return &cli;                                                                                    \
    }

#define CLI_INIT(var_name) cli_t const* var_name = cli_options_setup_global_gen()


#endif /* ifndef CLI_CLI_H_ */
