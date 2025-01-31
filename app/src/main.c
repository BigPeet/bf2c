#include <stdio.h>
#include <stdlib.h>

#include "app/config.h"
#include "bf2c/c_emitter.h"
#include "bf2c/optimize.h"
#include "bf2c/parser.h"
#include "bf2c/program.h"
#include "cli/cli.h"
#include "cli/error_codes.h"
#include "cli/param.h"
#include "cli/result.h"
#include "core/logging.h"

#ifdef NDEBUG
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO
#else
#define DEFAULT_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

CLI_SETUP(
    PROJECT_NAME,
    VERSION_MAJOR,
    VERSION_MINOR,
    VERSION_PATCH,
    "A Brainfuck to C transpiler",
    CLI_POSITIONAL_ARG("input", STRING, NULL, "\tInput file. Uses stdin, if not provided."),
    CLI_OPTION("output", 'o', "FILE", STRING, NULL, "\tOutput file. Uses stdout, if not provided."),
    CLI_OPTION("text", 't', "CODE", STRING, NULL, "\tInput Brainfuck code as a string."),
    COMMON_OPTIONS())

int main(int argc, char* argv[])
{
    LOGGING_INIT(DEFAULT_LOG_LEVEL);
    CLI_INIT(cli);

    { // scoped to minimize lifetime of variables
        cli_result_t res = cli_parse_args(cli, argc, argv);
        if (!cli_result_t_has_value(&res)) // !res.has_value
        {
            cli_error_t err = cli_result_t_unwrap_err(&res);
            cli_print_error(&err);
            cli_print_usage(cli);
            CLI_DEINIT(); // TODO: maybe combine this in a clean up function?
            return (int) err.error_code;
        }
    }
    // Apply verbosity settings
    if (cli_param_get_bool(cli_get_param_by_name(cli, "verbose")))
    {
        LOGGING_SET(LOG_LEVEL_DEBUG);
    }
    else if (cli_param_get_bool(cli_get_param_by_name(cli, "quiet")))
    {
        LOGGING_SET(LOG_LEVEL_ERROR);
    }

    int return_value = 0;
    // TODO: how to improve? Do I want macros?
    if (cli_param_get_bool(cli_get_param_by_name(cli, "help")))
    {
        cli_print_usage(cli);
    }
    else if (cli_param_get_bool(cli_get_param_by_name(cli, "version")))
    {
        cli_print_version(cli);
    }
    else
    {
        char const* input_file  = cli_param_get_string(cli_get_param_by_name(cli, "input"));
        char const* output_file = cli_param_get_string(cli_get_param_by_name(cli, "output"));
        char const* text        = cli_param_get_string(cli_get_param_by_name(cli, "text"));
        if (input_file && text)
        {
            LOG_ERROR_MSG("Specified both an input file and a text string. "
                          "Please specify only one of them.");
            LOG_DEBUG("Input file: %s, Text: %s", input_file, text);
            cli_print_usage(cli);
            CLI_DEINIT();
            return CLI_ERROR_INVALID_ARGUMENT;
        }

        LOG_DEBUG("Input: %s", input_file ? input_file : text ? "text" : "stdin");
        LOG_DEBUG("Output: %s", output_file ? output_file : "stdout");
        if (!input_file && !text)
        {
            LOG_INFO_MSG("Reading from stdin. Press Ctrl+D to finish.");
        }
        program_t prog = input_file ? bf2c_parse_file_by_name(input_file)
                         : text     ? bf2c_parse_text(text)
                                    : bf2c_parse_file(stdin);
        bf2c_optimize(&prog, BF2C_OPTIMIZATION_LEVEL_MEDIUM);
        return_value = output_file ? bf2c_emit_c_to_filename(output_file, &prog)
                                   : bf2c_emit_c_to_file(stdout, &prog);

        bf2c_program_destroy(&prog);
    }

    CLI_DEINIT();
    return return_value;
}
