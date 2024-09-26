#include <stdio.h>
#include <stdlib.h>
#include "cli/cli.h"
#include "cli/option.h"
#include "core/util/logging.h"

CLI_SETUP_OPTIONS(
    "bf2c",
    "A Brainf*ck to C transpiler",
    CLI_OPTION("input", 'i', "FILE", STRING, NULL, "\tInput file. Uses stdin, if not provided."),
    CLI_OPTION("output", 'o', "FILE", STRING, NULL, "\tOutput file. Uses stdout, if not provided."),
    CLI_FLAG("verbose", 'v', "\t\tVerbose output"),
    CLI_FLAG("dynamic-memory", 'd', "\tUse dynamic memory allocation"),
    CLI_FLAG("help", 'h', "\t\tPrint this help message"),
    CLI_FLAG("version", 0, "\t\tPrint version"), )

int main(int argc, char* argv[])
{
    LOGGING_INIT(LOGLEVEL_DEBUG);
    CLI_INIT_OPTIONS(cli);
    // TODO: implement parse_args function
    if (!cli_parse_args(cli, argc, argv))
    {
        cli_print_usage(cli);
        exit(EXIT_FAILURE);
    }
    printf("Input specified: %s\n", cli->options[0].given_value.STRING_value);
    return 0;
}
