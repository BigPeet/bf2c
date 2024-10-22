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
    ABORT_IF(!cli);
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

void cli_destroy(cli_t* cli)
{
    ABORT_IF(!cli);
    for (size_t i = 0; i < cli->options_len; ++i)
    {
        cli_option_destroy(&cli->options[i]);
    }
}

static cli_result_t cli_parse_parameters(cli_option_t* option, int* index, int argc, char** argv)
{
    assert(option);
    assert(index);
    assert(argv);

    if (*index >= argc - 1)
    {
        // There is no further user provided parameter.
        return CLI_ERR(CLI_ERROR_MISSING_PARAMETER, argv[*index]);
    }

    if (option->uses_multiple_values)
    {
        // Check how many arguments are provided
        // N.B.: strings in argv[i] are null-terminated, i.e. they containat least one char.
        int const start = *index + 1;
        int i           = start;
        while (i < argc && argv[i][0] != '-')
        {
            i++;
        }
        int const num_args = i - start;

        if (num_args <= 0)
        {
            // Next immediate argument starts with '-'.
            // For "single-value" options this will be set to the option value,
            // but an error is returned in this case, for now.
            return CLI_ERR(CLI_ERROR_MISSING_PARAMETER, argv[*index]);
        }

        if (num_args > 1)
        {
            // There ARE multiple values which we can set.
            *index += num_args;
            return cli_option_set_values(option, (size_t) num_args, argv + start);
        }

        // Exactly 1 argument is provided: Just treat this as single-value option.
        // Will be handled by if-block below.
        option->uses_multiple_values = false;
    }

    // At this point, the option should be a single-value option.
    assert(!option->uses_multiple_values);

    *index += 1;
    // Return afterwards. Nothing else to do for this option.
    return cli_option_set_value(option, argv[*index]);
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
            return cli_parse_parameters(&cli->options[j], index, argc, argv);
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
                    return cli_parse_parameters(&cli->options[j], index, argc, argv);
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
    ABORT_IF(!cli || !argv);

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
