#include "core/vector.h"

#include <stdint.h>

VECTOR_DEFINE_WITH_PREFIX(core_vec_int_t, core_vec_int, int32_t, void)
VECTOR_DEFINE_WITH_PREFIX(core_vec_char_t, core_vec_char, char, void)
VECTOR_DEFINE_WITH_PREFIX(core_vec_string_t, core_vec_string, char const*, void)
VECTOR_DEFINE_WITH_PREFIX(core_vec_double_t, core_vec_double, double, void)
