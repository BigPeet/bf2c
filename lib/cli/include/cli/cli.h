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
    char const* name;
    char const* description;
    struct cli_option_t* options;
    size_t options_len;
} cli_t;

void cli_print_usage(cli_t const* cli);
void cli_print_version(cli_t const* cli);
void cli_print_options(cli_t const* cli);
void cli_destroy(cli_t* cli);
cli_result_t cli_parse_args(cli_t const* cli, int argc, char** argv);


// User MACROS for setup and initialization
#define CLI_SETUP(program_name, major, minor, patch, desc, ...)                                  \
    static cli_t* cli_options_setup_global_gen(void)                                             \
    {                                                                                            \
        static cli_option_t options[] = {__VA_ARGS__};                                           \
        static cli_t cli              = {.name        = (program_name),                          \
                                         .description = (desc),                                  \
                                         .version     = {(major), (minor), (patch)},             \
                                         .options     = options,                                 \
                                         .options_len = sizeof(options) / sizeof(cli_option_t)}; \
        return &cli;                                                                             \
    }

#define CLI_INIT(var_name) cli_t const* var_name = cli_options_setup_global_gen()

#define CLI_DEINIT() cli_destroy(cli_options_setup_global_gen())


#endif /* ifndef CLI_CLI_H_ */
