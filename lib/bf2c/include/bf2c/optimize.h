#ifndef BF2C_OPTIMIZE_H_
#define BF2C_OPTIMIZE_H_

#include "bf2c/program.h"

typedef enum bf2c_optimization_level_t
{
    BF2C_OPTIMIZATION_LEVEL_NONE    = 0,
    BF2C_OPTIMIZATION_LEVEL_DEFAULT = 1,
    BF2C_OPTIMIZATION_LEVEL_MEDIUM  = 2,
    BF2C_OPTIMIZATION_LEVEL_MAX     = 3,
} bf2c_optimization_level_t;

// TODO: Would a return value make sense (for both functions)?
//       A result or boolean indicating changes?
void bf2c_optimize(program_t* program, bf2c_optimization_level_t level);
void bf2c_fold_constants(program_t* program);
void bf2c_remove_dead_code(program_t* program);
void bf2c_remove_noop(program_t* program);
void bf2c_loop_unrolling(program_t* program);

#endif /* ifndef BF2C_OPTIMIZE_H_ */
