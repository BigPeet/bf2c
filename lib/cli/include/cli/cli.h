#ifndef CLI_CLI_H_
#define CLI_CLI_H_

#include <stddef.h>
#include "cli/param.h"
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
    struct cli_param_t* parameters;
    size_t parameters_len;
} cli_t;

// Print functions
void cli_print_usage(cli_t const* cli);
void cli_print_version(cli_t const* cli);
void cli_print_parameters(cli_t const* cli);

// Initialization / Setter functions
cli_result_t cli_parse_args(cli_t const* cli, int argc, char** argv);

// Deinit functions
void cli_destroy(cli_t* cli);

// Accessor functions
// TODO: Should these be result-based? Should there be additional result-based APIs?
// E.g., see param.h
// TODO: What if the parameter does not exist? Return null? Abort? Different Functions for either?
// Since this here is more of "option" thing, I actually think returning a pointer might be
// fine...result might not be needed.
// But a panick-ing vs. return null API might still be interesting
cli_param_t const* cli_get_param_by_name(cli_t const* cli, char const* name);
cli_param_t const* cli_get_param_by_index(cli_t const* cli, size_t index);
cli_param_t const* cli_get_param_by_short_form(cli_t const* cli, char short_form);

// User MACROS for setup and initialization
#define CLI_SETUP(program_name, major, minor, patch, desc, ...)                                         \
    static cli_t* cli_gen_global_setup(void)                                                            \
    {                                                                                                   \
        static cli_param_t parameters[] = {__VA_ARGS__};                                                \
        static cli_t cli                = {.name           = (program_name),                            \
                                           .description    = (desc),                                    \
                                           .version        = {(major), (minor), (patch)},               \
                                           .parameters     = parameters,                                \
                                           .parameters_len = sizeof(parameters) / sizeof(cli_param_t)}; \
        return &cli;                                                                                    \
    }

#define CLI_INIT(var_name) cli_t const* var_name = cli_gen_global_setup()

#define CLI_DEINIT() cli_destroy(cli_gen_global_setup())


#endif /* ifndef CLI_CLI_H_ */
