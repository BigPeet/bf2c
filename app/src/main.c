#include <stdio.h>
#include <stdlib.h>

#include "cli/cli.h"
#include "cli/error_codes.h"
#include "cli/option.h"
#include "cli/result.h"
#include "core/logging.h"

CLI_SETUP(
    "bf2c",
    0,
    1,
    0,
    "A Brainfuck to C transpiler",
    CLI_OPTION("input", 'i', "FILE", STRING, NULL, "\tInput file. Uses stdin, if not provided."),
    CLI_OPTION("output", 'o', "FILE", STRING, NULL, "\tOutput file. Uses stdout, if not provided."),
    CLI_FLAG("dynamic-memory", 'd', "\tUse dynamic memory allocation."),
    CLI_FLAG("interpret", 0, "\tInterpret instead of transpiling."),
    COMMON_OPTIONS())

int main(int argc, char* argv[])
{
    LOGGING_INIT(LOG_LEVEL_DEBUG);
    CLI_INIT(cli);

    { // scoped to minimize lifetime of variables
        // TODO: implement parse_args function
        cli_result_t res = cli_parse_args(cli, argc, argv);
        if (!cli_result_t_has_value(&res)) // !res.has_value
        {
            cli_error_t err = cli_result_t_unwrap_err(&res);
            cli_print_error(err); // res.storage.error (unchecked)
            cli_print_usage(cli);
            exit((int) err);
        }
    }
    // TODO: implement better accessor functions
    if (cli->options[6].given_value.BOOL_value)
    {
        cli_print_usage(cli);
        exit(EXIT_SUCCESS);
    }
    if (cli->options[7].given_value.BOOL_value)
    {
        cli_print_version(cli);
        exit(EXIT_SUCCESS);
    }
    printf("Input specified: %s\n", cli->options[0].given_value.STRING_value);
    return 0;
}
