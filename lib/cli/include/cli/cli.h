#ifndef CLI_CLI_H_
#define CLI_CLI_H_

#include <stddef.h>

#include "cli/param.h"
#include "cli/result.h"

// cli_t: Struct and functions

typedef struct cli_version_t {
    int major;
    int minor;
    int patch;
    char const* extra_info;
} cli_version_t;

typedef struct cli_t {
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
// There are two variants of param-accessor functions:
// 1. cli_get_param_by_*: Will abort if parameter is not found, always returns a non-null pointer.
// 2. cli_try_get_param_by_*: Returns null if parameter is not found.
// The second API might be replaced with a "result-based" API in the future.
// TODO: If appropriate, add function which searches AND gets a value. Might lead to clutter/bloat.
cli_param_t const* cli_get_param_by_name(cli_t const* cli, char const* name);
cli_param_t const* cli_get_param_by_index(cli_t const* cli, size_t index);
cli_param_t const* cli_get_param_by_short_form(cli_t const* cli, char short_form);

cli_param_t const* cli_try_get_param_by_name(cli_t const* cli, char const* name);
cli_param_t const* cli_try_get_param_by_index(cli_t const* cli, size_t index);
cli_param_t const* cli_try_get_param_by_short_form(cli_t const* cli, char short_form);

// User MACROS for setup and initialization
#define CLI_VERSION(major_version, minor_version, patch_version, extra_information)                \
    {                                                                                              \
        .major = (major_version), .minor = (minor_version), .patch = (patch_version),              \
        .extra_info = (extra_information)                                                          \
    }

#define CLI_SETUP(program_name, desc, program_version, ...)                                             \
    static cli_t* cli_gen_global_setup(void) {                                                          \
        static cli_param_t parameters[] = {__VA_ARGS__};                                                \
        static cli_t cli                = {.name           = (program_name),                            \
                                           .description    = (desc),                                    \
                                           .version        = program_version,                           \
                                           .parameters     = parameters,                                \
                                           .parameters_len = sizeof(parameters) / sizeof(cli_param_t)}; \
        return &cli;                                                                                    \
    }

#define CLI_INIT(var_name) cli_t const* var_name = cli_gen_global_setup()

#define CLI_DEINIT() cli_destroy(cli_gen_global_setup())

#endif /* ifndef CLI_CLI_H_ */
