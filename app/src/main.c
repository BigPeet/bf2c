#include <stdio.h>
#include <stdlib.h>

#include "app/config.h"
#include "cli/cli.h"
#include "cli/error_codes.h"
#include "cli/param.h"
#include "cli/result.h"
#include "core/logging.h"

CLI_SETUP(
    PROJECT_NAME,
    VERSION_MAJOR,
    VERSION_MINOR,
    VERSION_PATCH,
    "A Brainfuck to C transpiler",
    CLI_MULTI_OPTION(
        "input", 'i', "FILES...", STRING, "\tInput files. Uses stdin, if not provided."),
    CLI_OPTION("output", 'o', "FILE", STRING, NULL, "\tOutput file. Uses stdout, if not provided."),
    CLI_FLAG("dynamic-memory", 'd', "\tUse dynamic memory allocation."),
    CLI_FLAG("interpret", 0, "\tInterpret instead of transpiling."),
    COMMON_OPTIONS())

int main(int argc, char* argv[])
{
    LOGGING_INIT(LOG_LEVEL_DEBUG);
    CLI_INIT(cli);

    { // scoped to minimize lifetime of variables
        cli_result_t res = cli_parse_args(cli, argc, argv);
        if (!cli_result_t_has_value(&res)) // !res.has_value
        {
            cli_error_t err = cli_result_t_unwrap_err(&res);
            cli_print_error(&err); // res.storage.error (unchecked)
            cli_print_usage(cli);
            CLI_DEINIT(); // TODO: maybe combine this in a clean up function?
            exit((int) err.error_code);
        }
    }

    // TODO: how to improve? Do I want macros?
    cli_print_parameters(cli);
    if (cli_param_get_bool(cli_get_param_by_name(cli, "help")))
    {
        cli_print_usage(cli);
    }
    else if (cli_param_get_bool_or(cli_get_param_by_short_form(cli, 'v'), false))
    {
        cli_print_version(cli);
    }
    else
    {
        // Accessing multi-value param
        cli_param_t const* const input_param = cli_get_param_by_short_form(cli, 'i');
        char const** input                   = cli_param_get_strings(input_param);
        printf("Input specified: ");
        for (size_t i = 0; i < input_param->values_len; ++i)
        {
            printf("%s ", input[i]);
        }
        printf("\n");
        free((void*) input);

        // Raw approach (no additional allocation)
        printf("(Raw) Input specified: ");
        cli_param_value_t const* input_values = cli_param_get_values(input_param);
        for (size_t i = 0; i < input_param->values_len; ++i)
        {
            printf("%s ", input_values[i].STRING_value); // unchecked
        }
        printf("\n");
    }

    CLI_DEINIT();
    return 0;
}
