#ifndef BF2C_COMMAND_H_
#define BF2C_COMMAND_H_

#include <stdint.h>

typedef enum command_type
{
    COMMAND_TYPE_INC_VAL,
    COMMAND_TYPE_DEC_VAL,
    COMMAND_TYPE_INC_PTR,
    COMMAND_TYPE_DEC_PTR,
    COMMAND_TYPE_OUT,
    COMMAND_TYPE_IN,
    COMMAND_TYPE_LOOP_START,
    COMMAND_TYPE_LOOP_END,
    COMMAND_TYPE_DEBUG,
} command_type;

typedef struct command_t
{
    int32_t value;
    command_type type;
} command_t;

#endif /* ifndef BF2C_COMMAND_H_ */
