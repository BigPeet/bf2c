#ifndef CLI_ERROR_CODES_H_
#define CLI_ERROR_CODES_H_

typedef enum cli_error_code_t
{
    CLI_ERROR = 1,
    CLI_ERROR_MISSING_PARAMETER,
    CLI_ERROR_INVALID_PARAMETER,
    CLI_ERROR_UNKNOWN_OPTION,
    CLI_ERROR_INVALID_OPTION_TYPE,
} cli_error_code_t;

typedef struct cli_error_t
{
  char const* extra_msg;
  cli_error_code_t error_code;
} cli_error_t;

#define CLI_ERROR(ec, msg) (cli_error_t){.extra_msg=(msg), .error_code=(ec)}

void cli_print_error(cli_error_t const* error);

#endif /* ifndef CLI_ERROR_CODES_H_ */
