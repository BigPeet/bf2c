#include "cli/cli.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "cli/error_codes.h"
#include "cli/option.h"
#include "cli/result.h"
#include "core/abort.h"

static void cli_version_print(cli_version_t const* version)
{
    assert(version);
    printf("%d.%d.%d", version->major, version->minor, version->patch);
}

void cli_print_usage(cli_t const* cli)
{
    assert(cli);
    printf("Usage: %s [options]\n\n%s\n\nOptions:\n", cli->name, cli->description);
    for (size_t i = 0; i < cli->options_len; ++i)
    {
        cli_option_print_usage(&cli->options[i]);
    }
}

void cli_print_version(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("%s ", cli->name);
    cli_version_print(&cli->version);
    printf("\n");
}

void cli_print_options(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("Options:\n");
    for (size_t i = 0; i < cli->options_len; ++i)
    {
        cli_option_print_value(&cli->options[i]);
    }
    printf("\n");
}

static cli_result_t cli_parse_long_option(
    cli_t const* cli, char const* argument, int* index, int argc, char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);
    for (size_t j = 0; j < cli->options_len; ++j)
    {
        if (cli_option_same_long_name(&cli->options[j], argument))
        {
            if (!cli->options[j].parameter)
            {
                // no parameter, so treat this as a flag
                return cli_option_enable_flag(&cli->options[j]);
            }

            if (*index >= argc - 1)
            {
                // There is no further user provided parameter
                return CLI_ERR(CLI_ERROR_MISSING_PARAMETER, argv[*index]);
            }

            *index += 1;
            return cli_option_set_value(&cli->options[j], argv[*index]);
        }
    }
    return CLI_ERR(CLI_ERROR_UNKNOWN_OPTION, argv[*index]);
}

static cli_result_t cli_parse_short_options(
    cli_t const* cli, char const* argument, int* index, int argc, char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);
    cli_result_t parse_res = CLI_OK();
    while (*argument && parse_res.has_value)
    {
        parse_res = CLI_ERR(CLI_ERROR_UNKNOWN_OPTION, argument);
        for (size_t j = 0; j < cli->options_len; ++j)
        {
            if (cli_option_same_short_name(&cli->options[j], *argument))
            {
                if (cli->options[j].parameter)
                {
                    // option expects a parameter
                    if (strlen(argument) > 1)
                    {
                        // option may not be directly followed by further short options
                        return CLI_ERR(CLI_ERROR_INVALID_OPTION_SEQUENCE, argv[*index]);
                    }
                    if (*index >= argc - 1)
                    {
                        // There is no further user provided parameter
                        return CLI_ERR(CLI_ERROR_MISSING_PARAMETER, argument);
                    }

                    *index += 1;
                    // Return afterwards. Nothing else to do for this option.
                    return cli_option_set_value(&cli->options[j], argv[*index]);
                }

                // no parameter, so treat this as a flag
                parse_res = cli_option_enable_flag(&cli->options[j]);
                break; // out of the for loop
            }
        }
        argument++;
    }
    return parse_res;
}

cli_result_t cli_parse_args(cli_t const* cli, int argc, char** argv)
{
    // assuming argc and argv are passed from main.
    // no checking applied
    ABORT_IF(!cli);

    cli_result_t parse_res = CLI_OK();
    for (int i = 1; i < argc && parse_res.has_value; ++i)
    {
        char const* arg      = argv[i];
        size_t const arg_len = strlen(arg);
        if (arg_len >= 2 && arg[0] == '-')
        {
            // Parsing an option
            if (arg[1] == '-')
            {
                // Long option
                if (arg_len == 2)
                {
                    // "--" is a special case, it means end of options
                    break;
                }
                parse_res = cli_parse_long_option(cli, arg + 2, &i, argc, argv);
            }
            else
            {
                // Short option
                parse_res = cli_parse_short_options(cli, arg + 1, &i, argc, argv);
            }
        }
        else
        {
            // TODO: positional arguments
            LOG_AND_ABORT("Positional arguments are not imlemented yet!");
        }
    }
    return parse_res;
}
