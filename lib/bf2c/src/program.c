#include "bf2c/program.h"

#include <stdio.h>
#include "bf2c/command.h"
#include "core/vector.h"

program_t bf2c_program_create(command_vec_t commands)
{
    return (program_t){commands};
}

void bf2c_program_destroy(program_t* program)
{
    if (program)
    {
        command_vec_destroy(&program->commands);
    }
}

void bf2c_program_print(program_t const* program)
{
    int padding = 0;
    size_t size = program->commands.size;
    while (size > 0)
    {
        padding++;
        size /= 10;
    }
    VEC_FOR_EACH(command_t, cmd, program->commands)
    {
        printf("[%*zu] Command: %s, Value: %d\n",
               padding,
               cmd_iterator,
               bf2c_command_type_to_string(cmd.type),
               cmd.value);
    }
}
