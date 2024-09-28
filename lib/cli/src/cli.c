#include "cli/cli.h"

#include <assert.h>
#include <stdio.h>
#include "cli/error_codes.h"
#include "cli/option.h"
#include "core/abort.h"
#include "core/result.h"

RESULT_VOID_DEFINE(cli_parse_result_t, cli_error_t)

static void cli_version_print(cli_version_t const* version)
{
    assert(version);
    printf("%d.%d.%d", version->major, version->minor, version->patch);
}

/*static char* cli_version_to_string(cli_version_t const* version)*/
/*{*/
/*assert(version);*/
/*char* buffer = malloc(32 * sizeof(char));*/
/*(void) snprintf(buffer,*/
/*32 * sizeof(char),*/
/*"%d.%d.%d",*/
/*version->major,*/
/*version->minor,*/
/*version->patch);*/
/*return buffer;*/
/*}*/

void cli_print_usage(cli_t const* cli)
{
    assert(cli);
    printf("Usage: %s [options]\n\n%s\n\nOptions:\n", cli->program_name, cli->program_description);
    for (size_t i = 0; i < cli->count; ++i)
    {
        cli_option_print(&cli->options[i]);
    }
}

void cli_print_version(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("%s ", cli->program_name);
    cli_version_print(&cli->version);
    printf("\n");
}

cli_parse_result_t cli_parse_long_option(
    cli_t const* cli, char const* argument, int* index, int argc, char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);
    for (size_t j = 0; j < cli->count; ++j)
    {
        if (cli_option_same_long_name(&cli->options[j], argument))
        {
            if (!cli->options[j].parameter)
            {
                // no parameter, so treat this as a flag
                cli_option_enable_flag(&cli->options[j]);
                return cli_parse_result_t_create_from_value();
            }

            if (*index >= argc - 1)
            {
                // There is no further user provided parameter
                return cli_parse_result_t_create_from_error(CLI_ERROR_MISSING_PARAMETER);
            }

            *index += 1;
            return cli_option_set_value(&cli->options[j], argv[*index])
                       ? cli_parse_result_t_create_from_value()
                       : cli_parse_result_t_create_from_error(CLI_ERROR_INVALID_PARAMETER);
        }
    }
    return cli_parse_result_t_create_from_error(CLI_ERROR_UNKNOWN_OPTION);
}

cli_parse_result_t cli_parse_args(cli_t const* cli, int argc, char** argv)
{
    // assuming argc and argv are passed from main.
    // no checking applied
    assert(cli);

    for (int i = 1; i < argc; ++i)
    {
        char* arg = argv[i];
        if (arg[0] == '-' && arg[1] == '-')
        {
            cli_parse_result_t res = cli_parse_long_option(cli, arg + 2, &i, argc, argv);
            if (!res.has_value)
            {
                return res;
            }
        }
        else if (arg[0] == '-')
        {
            // Short option
            /*char* option = arg + 1;*/
            /*for (size_t j = 0; j < cli->count; ++j)*/
            /*{*/
            /*if (strcmp(&cli->options[j].short_name, option) == 0)*/
            /*{*/
            /*cli->options[j].given_value.bool_value = true;*/
            /*break;*/
            /*}*/
            /*}*/
        }
        else
        {
            // TODO: positional arguments
            assert(false && "Positional arguments not supported yet");
        }
    }
    return cli_parse_result_t_create_from_value();
}
