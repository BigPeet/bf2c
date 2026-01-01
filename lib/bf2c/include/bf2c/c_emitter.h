#ifndef BF2C_C_EMITTER_H_
#define BF2C_C_EMITTER_H_

#include <stdbool.h>
#include <stdio.h>

#include "bf2c/program.h"

// TODO: return a RESULT for more precise error handling instead of bool
bool bf2c_emit_c_to_file(FILE* file, program_t const* program);
bool bf2c_emit_c_to_filename(char const* filename, program_t const* program);

#endif /* ifndef BF2C_C_EMITTER_H_ */
