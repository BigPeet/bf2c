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

static cli_result_t cli_parse_option(cli_t const* cli,
                                     char const* argument,
                                     bool (*cmp_func)(cli_option_t const*, char const*),
                                     int* index,
                                     int argc,
                                     char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);
    for (size_t j = 0; j < cli->count; ++j)
    {
        if (cmp_func(&cli->options[j], argument))
        {
            if (!cli->options[j].parameter)
            {
                // no parameter, so treat this as a flag
                return cli_option_enable_flag(&cli->options[j]);
            }

            if (*index >= argc - 1)
            {
                // There is no further user provided parameter
                return cli_result_t_create_from_error(
                    (cli_error_t){argv[*index], CLI_ERROR_MISSING_PARAMETER});
            }

            *index += 1;
            return cli_option_set_value(&cli->options[j], argv[*index]);
        }
    }
    return cli_result_t_create_from_error((cli_error_t){argv[*index], CLI_ERROR_UNKNOWN_OPTION});
}

cli_result_t cli_parse_args(cli_t const* cli, int argc, char** argv)
{
    // assuming argc and argv are passed from main.
    // no checking applied
    ABORT_IF(!cli);

    cli_result_t parse_res = cli_result_t_create_from_value();
    for (int i = 1; i < argc && parse_res.has_value; ++i)
    {
        char* arg = argv[i];
        if (strlen(arg) >= 2 && arg[0] == '-')
        {
            // Parsing an option
            bool const is_long_opt = arg[1] == '-';
            int const offset       = is_long_opt ? 2 : 1;
            bool (*cmp_func)(cli_option_t const*, char const*) =
                is_long_opt ? cli_option_same_long_name : cli_option_same_short_name;
            parse_res = cli_parse_option(cli, arg + offset, cmp_func, &i, argc, argv);
        }
        else
        {
            // TODO: positional arguments
            LOG_AND_ABORT("Positional arguments are not imlemented yet!");
        }
    }
    return parse_res;
}
