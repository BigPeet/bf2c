#ifndef CLI_OPTIONS_H_
#define CLI_OPTIONS_H_

#include <stdbool.h>
#include <stdio.h>

typedef union cli_option_value_t
{
  double DOUBLE_value;
  char const* STRING_value;
  int INT_value;
  bool BOOL_value;
} cli_option_value_t;


typedef enum cli_option_value_type_t
{
  STRING,
  INT,
  DOUBLE,
  BOOL,
} cli_option_value_type_t;


typedef struct cli_option_t
{
  cli_option_value_t default_value;
  cli_option_value_t given_value;
  char const* long_name;
  char const* short_name;
  char const* parameter;
  char const* description;
  cli_option_value_type_t value_type;
} cli_option_t;


typedef struct cli_options_t
{
  size_t count;
  char const* program_name;
  char const* program_description;
  struct cli_option_t* options;
} cli_options_t;

void print_usage(cli_options_t const* options);

#define OPTION(name, short_form, param, desc, type, default_val)               \
  {                                                                            \
    .long_name = (name), .short_name = (short_form), .parameter = (param),     \
    .description = (desc), .default_value = { .type##_value = (default_val) }, \
    .value_type = (type), .given_value = {                                     \
      0                                                                        \
    }                                                                          \
  }


#define FLAG(name, short_form, desc) \
  OPTION(name, short_form, NULL, desc, BOOL, false)


#define INIT_OPTIONS(options_var, name, desc, ...)             \
  static cli_option_t _internal_arr[] = { __VA_ARGS__ };       \
  static cli_options_t options_var = { .program_name = (name), \
    .program_description = (desc),                             \
    .count = sizeof(_internal_arr) / sizeof(cli_option_t),      \
    .options = _internal_arr }


#endif /* ifndef CLI_OPTIONS_H_ */
