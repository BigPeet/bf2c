#include <stdio.h>
#include <stdlib.h>
#include "cli/cli.h"
#include "cli/option.h"
#include "core/logging.h"

CLI_SETUP(
    "bf2c",
    0,
    1,
    0,
    "A Brainfuck to C transpiler",
    CLI_OPTION("input", 'i', "FILE", STRING, NULL, "\tInput file. Uses stdin, if not provided."),
    CLI_OPTION("output", 'o', "FILE", STRING, NULL, "\tOutput file. Uses stdout, if not provided."),
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"),
    CLI_FLAG("quiet", 'q', "\t\tQuiet mode"),
    CLI_FLAG("dynamic-memory", 'd', "\tUse dynamic memory allocation"),
    CLI_FLAG("help", 'h', "\t\tPrint this help message"),
    CLI_FLAG("version", 0, "\t\tPrint version"), )

int main(int argc, char* argv[])
{
    LOGGING_INIT(LOG_LEVEL_DEBUG);
    CLI_INIT(cli);
    // TODO: implement parse_args function
    if (!cli_parse_args(cli, argc, argv))
    {
        cli_print_usage(cli);
        exit(EXIT_FAILURE);
    }
    // TODO: implement better accessor functions
    if (cli->options[5].given_value.BOOL_value)
    {
        cli_print_usage(cli);
        exit(EXIT_SUCCESS);
    }
    if (cli->options[6].given_value.BOOL_value)
    {
        cli_print_version(cli);
        exit(EXIT_SUCCESS);
    }
    printf("Input specified: %s\n", cli->options[0].given_value.STRING_value);
    return 0;
}
