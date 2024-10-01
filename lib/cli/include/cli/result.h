#ifndef CLI_RESULT_H_
#define CLI_RESULT_H_

#include "cli/error_codes.h"
#include "core/result.h"

RESULT_VOID_DECLARE(cli_result_t, cli_error_t)

// Helper macros

// Create a successful result
#define CLI_OK() cli_result_t_create_from_value()
// Create a result from an error
#define CLI_ERR(err_expr) cli_result_t_create_from_error(err_expr)

#endif /* ifndef CLI_RESULT_H_ */
