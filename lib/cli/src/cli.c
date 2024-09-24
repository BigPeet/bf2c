#include "cli/cli.h"

#include <assert.h>
#include <stdio.h>
#include "cli/options.h"


void cli_print_usage(cli_t const* cli)
{
    assert(cli);
    printf("Usage: %s [options]\n\n%s\n\nOptions:\n", cli->program_name, cli->program_description);
    for (size_t i = 0; i < cli->count; ++i)
    {
        cli_option_print(&cli->options[i]);
    }
}

static bool cli_parse_long_option(
    cli_t const* cli, char const* argument, int* index, int argc, char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);
    for (size_t j = 0; j < cli->count; ++j)
    {
        if (cli_option_has_long_name(&cli->options[j], argument))
        {
            if (!cli->options[j].parameter)
            {
                // no parameter, so treat this as a flag
                cli_option_enable_flag(&cli->options[j]);
                return true;
            }

            if (*index >= argc)
            {
                // missing parameter
                return false;
            }

            *index += 1;
            return cli_option_set_value(&cli->options[j], argv[*index]);
        }
    }
    return false;
}

bool cli_parse_args(cli_t const* cli, int argc, char** argv)
{
    // assuming argc and argv are passed from main.
    // no checking applied
    assert(cli);

    for (int i = 1; i < argc; ++i)
    {
        char* arg = argv[i];
        if (arg[0] == '-' && arg[1] == '-' && !cli_parse_long_option(cli, arg + 2, &i, argc, argv))
        {
            return false;
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
    return true;
}
