#ifndef CLI_PARAM_H_
#define CLI_PARAM_H_

#include <stdbool.h>
#include "cli/result.h"

// forward declaration
union cli_param_value_t;

typedef union cli_param_value_t
{
    double DOUBLE_value;
    char const* STRING_value;
    int INT_value;
    bool BOOL_value;

    // Alternatively, we could store arrays of the specific types.
    // That COULD be more efficient, but would make the API more complex.
    union cli_param_value_t* values;
} cli_param_value_t;


typedef enum cli_param_value_type_t
{
    STRING,
    INT,
    DOUBLE,
    BOOL,
} cli_param_value_type_t;


typedef struct cli_param_t
{
    cli_param_value_t contained;
    size_t values_len;
    char const* long_name;
    char const* arg_name;
    char const* description;
    cli_param_value_type_t value_type;
    char short_name;
    // Using bitfields to pack the booleans into a single byte
    bool uses_multiple_values : 1;    // this may change during parsing based on actual usage
    bool may_use_multiple_values : 1; // this is a static property
    bool is_positional : 1;           // this is a static property
    bool is_set_by_user : 1;          // this may change during parsing based on actual usage
} cli_param_t;

// Print functions
void cli_param_print_usage(cli_param_t const* param);
void cli_param_print_value(cli_param_t const* param);

// Utility functions
bool cli_param_same_long_name(cli_param_t const* param, char const* name);
bool cli_param_same_short_name(cli_param_t const* param, char name);

// Initialization / Setter functions
cli_result_t cli_param_enable_flag(cli_param_t* param);
cli_result_t cli_param_set_value(cli_param_t* param, char const* argument);
cli_result_t cli_param_set_values(cli_param_t* param, size_t num_values, char** arguments);

// Deinit / Clear functions
void cli_param_destroy(cli_param_t* param);

// Accessor functions
// TODO: Accessors should have these general APIs available
//
// 1. UWRAP: Return the double/int/... value(s), panic on error.
// 2. TRY: Write the double/int/... value(s) into OUT parameter, return false on error or a result
//         Or introduce results for double/int/... and return those
// 3. UNCHECKED: Return the double/int/... value(s) without checking for errors or UB
// 4. GET_OR: Return the double/int/... value(s) or a default value
//               Could be redundant with a return-result-based version for TRY
//               cli_param_bool_res_unwrap_or(cli_param_get_bool(param), false);
//               cli_param_get_bool_or(param, false);

// Abort if the parameter does not contain the expected type
double cli_param_unwrap_double(cli_param_t const* param);
char const* cli_param_unwrap_string(cli_param_t const* param);
int cli_param_unwrap_int(cli_param_t const* param);
bool cli_param_unwrap_bool(cli_param_t const* param);

double const* cli_param_unwrap_doubles(cli_param_t const* param);
char const* const* cli_param_unwrap_strings(cli_param_t const* param);
int const* cli_param_unwrap_ints(cli_param_t const* param);
bool const* cli_param_unwrap_bools(cli_param_t const* param);





// MACROs for convenience
#define CLI_OPTION(name, short_form, param, type, default_val, desc)                        \
    {                                                                                       \
        .contained = {.type##_value = (default_val)}, .values_len = 0, .long_name = (name), \
        .arg_name = (param), .description = (desc), .value_type = (type),                   \
        .short_name = (short_form), .uses_multiple_values = false,                          \
        .may_use_multiple_values = false, .is_positional = false, .is_set_by_user = false,  \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define CLI_MULTI_OPTION(name, short_form, param, type, desc)                                     \
    {                                                                                             \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = (param), \
        .description = (desc), .value_type = (type), .short_name = (short_form),                  \
        .uses_multiple_values = true, .may_use_multiple_values = true, .is_positional = false,    \
        .is_set_by_user = false,                                                                  \
    }

#define CLI_POSITIONAL_ARG(name, type, default_val, desc)                                       \
    {                                                                                           \
        .contained = {.type##_value = (default_val)}, .values_len = 0, .long_name = (name),     \
        .arg_name = NULL, .description = (desc), .value_type = (type), .short_name = '\0',      \
        .uses_multiple_values = false, .may_use_multiple_values = false, .is_positional = true, \
        .is_set_by_user = false,                                                                \
    }

#define CLI_POSITIONAL_MULTI_ARG(name, type, desc)                                             \
    {                                                                                          \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = NULL, \
        .description = (desc), .value_type = (type), .short_name = '\0',                       \
        .uses_multiple_values = true, .may_use_multiple_values = true, .is_positional = true,  \
        .is_set_by_user = false,                                                               \
    }

#define COMMON_OPTIONS()                                                                    \
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"), \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                              \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#endif /* ifndef CLI_PARAM_H_ */
