#ifndef CLI_PARAM_H_
#define CLI_PARAM_H_

#include <stdbool.h>

#include "cli/result.h"

// forward declaration
union cli_param_value_t;

typedef union cli_param_value_t {
    // NOLINTBEGIN(readability-identifier-naming)
    // Deviating from naming convention to match the naming of the types.
    // This is used in MACROs to initialize/select the correct value.
    double DOUBLE_value;
    char const* STRING_value;
    int INT_value;
    bool BOOL_value;
    // NOLINTEND(readability-identifier-naming)

    // Alternatively, we could store arrays of the specific types.
    // That COULD be more efficient, but would make the API more complex.
    union cli_param_value_t* values;
} cli_param_value_t;

typedef enum cli_param_value_type_t {
    STRING,
    INT,
    DOUBLE,
    BOOL,
} cli_param_value_type_t;

typedef struct cli_param_t {
    cli_param_value_t contained;
    size_t values_len;
    char const* long_name;
    char const* arg_name;
    char const* description;
    cli_param_value_type_t value_type;
    char short_name;
    // Using bitfields to pack the booleans into a single byte
    bool uses_multiple_values   : 1; // this may change during parsing based on actual usage
    bool may_use_multiple_values: 1; // this is a static property
    bool is_positional          : 1; // this is a static property
    bool is_set_by_user         : 1; // this may change during parsing based on actual usage
} cli_param_t;

// Print functions
void cli_param_print_usage(cli_param_t const* param);
void cli_param_print_value(cli_param_t const* param);

// Utility functions
bool cli_param_same_long_name(cli_param_t const* param, char const* name, size_t max_len);
bool cli_param_same_short_name(cli_param_t const* param, char name);

// Initialization / Setter functions
cli_result_t cli_param_enable_flag(cli_param_t* param);
cli_result_t cli_param_set_value(cli_param_t* param, char const* argument);
cli_result_t cli_param_set_values(cli_param_t* param, size_t num_values, char** arguments);

// Deinit / Clear functions
void cli_param_destroy(cli_param_t* param);

// Accessor functions

// Accessing the "raw" value(s) of the parameter (still owned by the parameter).
// This is either the single value or the array of values.
// This can be differentiated by the uses_multiple_values / values_len field of the parameter.
cli_param_value_t const* cli_param_get_values(cli_param_t const* param);

// Abort if the parameter does not contain the expected type
double cli_param_get_double(cli_param_t const* param);
char const* cli_param_get_string(cli_param_t const* param);
int cli_param_get_int(cli_param_t const* param);
bool cli_param_get_bool(cli_param_t const* param);

// Abort if the parameter does not contain the expected type or is not an array
// Returned arrays are owned by the caller and must be freed (but not their contents in case of
// strings).
// Might be NULL if the parameter does not contain any values.
// Number of values is stored in the values_len field of the parameter.
// TODO: Rename functions to make ownership clearer.
double const* cli_param_get_doubles(cli_param_t const* param);
char const** cli_param_get_strings(cli_param_t const* param);
int const* cli_param_get_ints(cli_param_t const* param);
bool const* cli_param_get_bools(cli_param_t const* param);


// Return the value or a default value
double cli_param_get_double_or(cli_param_t const* param, double value);
char const* cli_param_get_string_or(cli_param_t const* param, char const* value);
int cli_param_get_int_or(cli_param_t const* param, int value);
bool cli_param_get_bool_or(cli_param_t const* param, bool value);

// TODO: Add "_or" versions for the array types, if appropriate.

// TODO: implement "TRY" versions of the above functions which either
// - return a bool and take an out parameter to write into
// - e.g. bool cli_param_try_get_double(cli_param_t const* param, double* out);
// - return a result-type containing the value or error
// - e.g. cli_double_res_t cli_param_get_double_res(cli_param_t const* param);

// TODO: implement "unchecked" functions which return the value without checking

// FIXME: move macros into cli.h?

// MACROs for convenience
#define CLI_OPTION(name, short_form, param, type, default_val, desc)                               \
    {                                                                                              \
        .contained = {.type##_value = (default_val)}, .values_len = 1, .long_name = (name),        \
        .arg_name = (param), .description = (desc), .value_type = (type),                          \
        .short_name = (short_form), .uses_multiple_values = false,                                 \
        .may_use_multiple_values = false, .is_positional = false, .is_set_by_user = false,         \
    }

#define CLI_FLAG(name, short_form, desc) CLI_OPTION(name, short_form, NULL, BOOL, false, desc)

#define CLI_MULTI_OPTION(name, short_form, param, type, desc)                                      \
    {                                                                                              \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = (param),  \
        .description = (desc), .value_type = (type), .short_name = (short_form),                   \
        .uses_multiple_values = true, .may_use_multiple_values = true, .is_positional = false,     \
        .is_set_by_user = false,                                                                   \
    }

#define CLI_POSITIONAL_ARG(name, type, default_val, desc)                                          \
    {                                                                                              \
        .contained = {.type##_value = (default_val)}, .values_len = 1, .long_name = (name),        \
        .arg_name = NULL, .description = (desc), .value_type = (type), .short_name = '\0',         \
        .uses_multiple_values = false, .may_use_multiple_values = false, .is_positional = true,    \
        .is_set_by_user = false,                                                                   \
    }

#define CLI_POSITIONAL_MULTI_ARG(name, type, desc)                                                 \
    {                                                                                              \
        .contained = {.values = NULL}, .values_len = 0, .long_name = (name), .arg_name = NULL,     \
        .description = (desc), .value_type = (type), .short_name = '\0',                           \
        .uses_multiple_values = true, .may_use_multiple_values = true, .is_positional = true,      \
        .is_set_by_user = false,                                                                   \
    }

#define COMMON_OPTIONS()                                                                           \
    CLI_FLAG("verbose", 'v', "\t\tVerbose mode"), CLI_FLAG("quiet", 'q', "\t\tQuiet mode"),        \
        CLI_FLAG("help", 'h', "\t\tPrint this help message."),                                     \
        CLI_FLAG("version", 'V', "\t\tPrint version.")


#endif /* ifndef CLI_PARAM_H_ */
