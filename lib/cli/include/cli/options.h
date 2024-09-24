#ifndef CLI_OPTIONS_H_
#define CLI_OPTIONS_H_

#include <stdbool.h>

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
    char short_name;
    char const* parameter;
    char const* description;
    cli_option_value_type_t value_type;
} cli_option_t;

void cli_option_print(cli_option_t const* option);
bool cli_option_has_long_name(cli_option_t const* option, char const* name);
bool cli_option_has_short_name(cli_option_t const* option, char name);
void cli_option_enable_flag(cli_option_t* option);
bool cli_option_set_value(cli_option_t* option, char const* parameter);


#endif /* ifndef CLI_OPTIONS_H_ */
