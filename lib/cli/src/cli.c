#include "cli/cli.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "cli/error_codes.h"
#include "cli/param.h"
#include "cli/result.h"
#include "core/abort.h"

static void cli_version_print(cli_version_t const* version)
{
    assert(version);
    // FIXME: single printf? or better yet: use a stringstream-like approach?
    // I.e., build the string in memory, then print (and optionally consume) it.
    // This would also allow to return the version string instead of printing it directly.
    // Could be used throughout the library.
    printf("%d.%d.%d", version->major, version->minor, version->patch);
    if (version->extra_info && *version->extra_info)
    {
        printf(" %s", version->extra_info);
    }
}

void cli_print_usage(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("Usage: %s [options] ", cli->name);

    // Print positional parameters
    int num_positional = 0;
    for (size_t i = 0; i < cli->parameters_len; ++i)
    {
        if (cli->parameters[i].is_positional)
        {
            bool requires_separator = false;
            if (cli->parameters[i].may_use_multiple_values)
            {
                // check if this is the last positional parameter
                for (size_t j = i + 1; j < cli->parameters_len; ++j)
                {
                    if (cli->parameters[j].is_positional)
                    {
                        requires_separator = true;
                        break;
                    }
                }
            }
            printf("<%s%s> %s",
                   cli->parameters[i].long_name,
                   cli->parameters[i].may_use_multiple_values ? "..." : "",
                   requires_separator ? "[--] " : "");
            num_positional++;
        }
    }

    // Print program description
    printf("\n\n%s\n", cli->description);

    // Print Argument and Option descriptions
    if (num_positional > 0)
    {
        printf("\nArguments:\n");
        for (size_t i = 0; i < cli->parameters_len; ++i)
        {
            if (cli->parameters[i].is_positional)
            {
                cli_param_print_usage(&cli->parameters[i]);
            }
        }
    }
    printf("\nOptions:\n");
    for (size_t i = 0; i < cli->parameters_len; ++i)
    {
        if (!cli->parameters[i].is_positional)
        {
            cli_param_print_usage(&cli->parameters[i]);
        }
    }
}

void cli_print_version(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("%s ", cli->name);
    cli_version_print(&cli->version);
    printf("\n");
}

void cli_print_parameters(cli_t const* cli)
{
    ABORT_IF(!cli);
    printf("Parameters:\n");
    for (size_t i = 0; i < cli->parameters_len; ++i)
    {
        cli_param_print_value(&cli->parameters[i]);
    }
    printf("\n");
}

void cli_destroy(cli_t* cli)
{
    ABORT_IF(!cli);
    for (size_t i = 0; i < cli->parameters_len; ++i)
    {
        cli_param_destroy(&cli->parameters[i]);
    }
}

static cli_result_t cli_parse_param_arguments(cli_param_t* param, int* index, int argc, char** argv)
{
    assert(param);
    assert(index);
    assert(argv);

    if (*index >= argc - 1)
    {
        // There is no further user provided parameter.
        return CLI_ERR(CLI_ERROR_MISSING_ARGUMENT, argv[*index]);
    }

    if (param->may_use_multiple_values)
    {
        // Check how many arguments are provided
        // N.B.: strings in argv[i] are null-terminated, i.e. they contain at least one char.
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
            // For "single-value" parameters this will be set to the argument value,
            // but an error is returned in this case, for now.
            return CLI_ERR(CLI_ERROR_MISSING_ARGUMENT, argv[*index]);
        }

        if (num_args > 1)
        {
            // There ARE multiple values which we can set.
            *index += num_args;
            return cli_param_set_values(param, (size_t) num_args, argv + start);
        }

        // Exactly 1 argument is provided: Just treat this as single-value parameter.
        // Will be handled below.
    }

    *index += 1;
    // Return afterwards. Nothing else to do for this parameter.
    return cli_param_set_value(param, argv[*index]);
}

static cli_result_t cli_apply_value_to_param(cli_param_t* param,
                                             char const* argument,
                                             int* index,
                                             int argc,
                                             char** argv,
                                             char const* assigned)
{
    assert(param);

    if (param->is_set_by_user)
    {
        // already set previously
        return CLI_ERR(CLI_ERROR_DUPLICATE_PARAMETER, argument);
    }
    if (!param->arg_name)
    {
        // takes no arguments
        // check if a value was assigned regardless
        if (assigned && *assigned)
        {
            return CLI_ERR(CLI_ERROR_INVALID_ARGUMENT, argument);
        }
        // we treat this as a flag
        return cli_param_enable_flag(param);
    }
    if (assigned)
    {
        return cli_param_set_value(param, assigned);
    }
    // parse arguments of parameter
    return cli_parse_param_arguments(param, index, argc, argv);
}

static cli_result_t cli_parse_long_option(
    cli_t const* cli, char const* argument, int* index, int argc, char** argv)
{
    assert(cli);
    assert(argument);
    assert(index);

    char const* const assignment = strchr(argument, '=');
    size_t const split           = (assignment != NULL) ? (size_t) (assignment - argument) : 0;
    char const* const assigned   = (assignment != NULL) ? assignment + 1 : NULL;

    for (size_t j = 0; j < cli->parameters_len; ++j)
    {
        cli_param_t* param = &cli->parameters[j];
        assert(param);
        if (!param->is_positional && cli_param_same_long_name(param, argument, split))
        {
            // We found the matching parameter.
            return cli_apply_value_to_param(param, argument, index, argc, argv, assigned);
        }
    }
    return CLI_ERR(CLI_ERROR_UNKNOWN_PARAMETER, argv[*index]);
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
        // Treat the remainder (if any) as potential assigned value.
        char const* const assigned = *(argument + 1) ? argument + 1 : NULL;
        parse_res                  = CLI_ERR(CLI_ERROR_UNKNOWN_PARAMETER, argument);
        for (size_t j = 0; j < cli->parameters_len; ++j)
        {
            cli_param_t* param = &cli->parameters[j];
            if (!param->is_positional && cli_param_same_short_name(param, *argument))
            {
                // We found the matching parameter.
                parse_res = cli_apply_value_to_param(param,
                                                     argument,
                                                     index,
                                                     argc,
                                                     argv,
                                                     param->arg_name ? assigned : NULL);
                if (assigned && param->arg_name)
                {
                    return parse_res; // we've already consumed the remainder of the argument
                }
                break; // out of the for loop
            }
        }
        argument++;
    }
    return parse_res;
}

static cli_result_t cli_parse_positional_argument(cli_t const* cli,
                                                  int* index,
                                                  int argc,
                                                  char** argv)
{
    assert(cli);
    assert(index);

    for (size_t j = 0; j < cli->parameters_len; ++j)
    {
        if (cli->parameters[j].is_positional && !cli->parameters[j].is_set_by_user)
        {
            // decrement index to "simulate" an option before the position parameter
            *index -= 1;
            bool const accepts_multiple_inputs = cli->parameters[j].uses_multiple_values;
            cli_result_t const parse_res =
                cli_parse_param_arguments(&cli->parameters[j], index, argc, argv);
            if (accepts_multiple_inputs && cli_result_t_has_value(&parse_res))
            {
                // if last argument of positional parameter is followed by '--', consume it
                if (index && *index < argc - 1 && strcmp(argv[*index + 1], "--") == 0)
                {
                    *index += 1;
                }
            }
            return parse_res;
        }
    }
    return CLI_ERR(CLI_ERROR_UNKNOWN_PARAMETER, argv[*index]);
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
        // Case 1: Long option, e.g. "--foo"
        if (arg_len >= 3 && arg[0] == '-' && arg[1] == '-')
        {
            parse_res = cli_parse_long_option(cli, arg + 2, &i, argc, argv);
        }
        // Case 2: Short option(s), e.g. "-f" or "-fV"
        else if (arg_len >= 2 && arg[0] == '-' && arg[1] != '-')
        {
            parse_res = cli_parse_short_options(cli, arg + 1, &i, argc, argv);
        }
        // Case 3: Positional argument
        else if (arg[0] != '-')
        {
            parse_res = cli_parse_positional_argument(cli, &i, argc, argv);
        }
        // Case 4: Error, invalid/unknown parameter, e.g. "-" or "--".
        else
        {
            parse_res = CLI_ERR(CLI_ERROR_UNKNOWN_PARAMETER, argv[i]);
        }
    }
    return parse_res;
}

cli_param_t const* cli_try_get_param_by_name(cli_t const* cli, char const* name)
{
    if (cli && name)
    {
        for (size_t i = 0; i < cli->parameters_len; ++i)
        {
            if (cli_param_same_long_name(&cli->parameters[i], name, 0))
            {
                return &cli->parameters[i];
            }
        }
    }
    return NULL;
}

cli_param_t const* cli_get_param_by_name(cli_t const* cli, char const* name)
{
    ABORT_IF(!cli || !name);
    cli_param_t const* const param = cli_try_get_param_by_name(cli, name);
    if (!param)
    {
        LOG_AND_ABORT("Could not find parameter named '%s'.", name);
    }
    return param;
}

cli_param_t const* cli_try_get_param_by_index(cli_t const* cli, size_t index)
{
    if (cli && index < cli->parameters_len)
    {
        return &cli->parameters[index];
    }
    return NULL;
}

cli_param_t const* cli_get_param_by_index(cli_t const* cli, size_t index)
{
    ABORT_IF(!cli);
    cli_param_t const* const param = cli_try_get_param_by_index(cli, index);
    if (!param)
    {
        LOG_AND_ABORT("Parameter index %zu out of bounds. (Number of parameters: %zu)",
                      index,
                      cli->parameters_len);
    }
    return param;
}

cli_param_t const* cli_try_get_param_by_short_form(cli_t const* cli, char short_form)
{
    if (cli)
    {
        for (size_t i = 0; i < cli->parameters_len; ++i)
        {
            if (cli_param_same_short_name(&cli->parameters[i], short_form))
            {
                return &cli->parameters[i];
            }
        }
    }
    return NULL;
}

cli_param_t const* cli_get_param_by_short_form(cli_t const* cli, char short_form)
{
    ABORT_IF(!cli);
    cli_param_t const* const param = cli_try_get_param_by_short_form(cli, short_form);
    if (!param)
    {
        LOG_AND_ABORT("Could not find parameter with short form '%c'.", short_form);
    }
    return param;
}
