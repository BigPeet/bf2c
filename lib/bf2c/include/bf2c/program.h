#ifndef BF2C_PROGRAM_H_
#define BF2C_PROGRAM_H_

#include <stddef.h>
#include "bf2c/command.h"

typedef struct program_t
{
    command_vec_t commands;
} program_t;

program_t bf2c_program_create(command_vec_t commands);
void bf2c_program_destroy(program_t* program);
void bf2c_program_print(program_t const* program);

#endif /* ifndef BF2C_PROGRAM_H_ */
