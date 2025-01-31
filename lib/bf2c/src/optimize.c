#include "bf2c/optimize.h"

#include <stddef.h>
#include "bf2c/command.h"
#include "bf2c/program.h"
#include "core/abort.h"
#include "core/logging.h"

enum
{
    MAX_ITERATIONS = 1
};

void bf2c_optimize(program_t* program, bf2c_optimization_level_t level)
{
    for (size_t i = 0; i < MAX_ITERATIONS; ++i)
    {
        LOG_DEBUG("Optimization iteration %zu", i);
        switch (level)
        {
            case BF2C_OPTIMIZATION_LEVEL_MAX:
                // fallthrough
            case BF2C_OPTIMIZATION_LEVEL_MEDIUM:
                bf2c_fold_constants(program);
                // fallthrough
            case BF2C_OPTIMIZATION_LEVEL_DEFAULT:
                bf2c_remove_dead_code(program);
                bf2c_remove_noop(program);
                // fallthrough
            case BF2C_OPTIMIZATION_LEVEL_NONE:
                break;
        }
    }
}

static bool is_io_command(command_type type)
{
    return type == COMMAND_TYPE_IN || type == COMMAND_TYPE_OUT;
}

void bf2c_fold_constants(program_t* program)
{
    ABORT_IF(!program);
    // Between I/O operations, the remaining operations can be performed as-if.
    // E.g. `>+++++++++[<-------->-]<.` basically calculates `9 * 8` and adds it to the "starting
    // cell". So, I know that this could be shortened to `data[idx] += 72; /* := 'H' */`
    // (Loops can only be unrolled, if the involved cells contain known values or if the loop
    // trivially decrements the cell to 0.) For every such range between I/O, ...
    // - I can calculate which cells (relative to the current cell) are affected in which way, and
    // - I can calculate the new current cell.
    // I can then replace the range with equivalent commands (adding to the cell, moving if
    // necessary, repeat).

    command_vec_t new_commands = command_vec_create();

    for (size_t i = 0; i < program->commands.size; ++i)
    {
        if (is_io_command(program->commands.data[i].type))
        {
            // Hit I/O boundary. Summarize effects of commands since last I/O (or beginning).
            continue; // TODO
        }
        LOG_DEBUG("Command: %s %d",
                  bf2c_command_type_to_string(program->commands.data[i].type),
                  program->commands.data[i].value);
    }
}

void bf2c_remove_dead_code(program_t* program)
{
    ABORT_IF(!program);
    // Dead code is code that is never executed.
    // E.g., a loop that is never entered.
}

void bf2c_remove_noop(program_t* program)
{
    ABORT_IF(!program);
    // If a sequence of commands cancel each other out, they can be removed.
    // The corresponding command will have a value of 0.
}
