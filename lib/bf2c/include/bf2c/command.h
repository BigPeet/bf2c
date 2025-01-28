#ifndef BF2C_COMMAND_H_
#define BF2C_COMMAND_H_

#include <stdint.h>

#include "bf2c/token.h"
#include "core/vector.h"

typedef enum command_type
{
    COMMAND_TYPE_CHANGE_VAL,
    COMMAND_TYPE_CHANGE_PTR,
    COMMAND_TYPE_OUT,
    COMMAND_TYPE_IN,
    COMMAND_TYPE_LOOP_START,
    COMMAND_TYPE_LOOP_END,
    COMMAND_TYPE_DEBUG,
    COMMAND_TYPE_UNKNOWN,
} command_type;

typedef struct command_t
{
    int32_t value;
    command_type type;
} command_t;

command_type bf2c_command_from_token(token_type token);
int32_t bf2c_command_value(token_type token);
char const* bf2c_command_type_to_string(command_type type);

VECTOR_DECLARE_WITH_PREFIX(command_vec_t, command_vec, command_t, void)

#endif /* ifndef BF2C_COMMAND_H_ */
