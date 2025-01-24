#ifndef BF2C_PROGRAM_H_
#define BF2C_PROGRAM_H_

#include <stddef.h>
#include "bf2c/command.h"

typedef struct program_t
{
    command_t* commands;
    size_t commands_len;
} program_t;

#endif /* ifndef BF2C_PROGRAM_H_ */
