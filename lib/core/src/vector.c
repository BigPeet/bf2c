#include "core/vector.h"

#include <stdint.h>
#include <string.h>

VECTOR_DEFINE_WITH_PREFIX(core_vec_int_t, core_vec_int, int32_t, void, TRIVIAL_COMP)
VECTOR_DEFINE_WITH_PREFIX(core_vec_size_t, core_vec_size, size_t, void, TRIVIAL_COMP)
VECTOR_DEFINE_WITH_PREFIX(core_vec_char_t, core_vec_char, char, void, TRIVIAL_COMP)
VECTOR_DEFINE_WITH_PREFIX(core_vec_string_t, core_vec_string, char const*, void, strcmp)
VECTOR_DEFINE_WITH_PREFIX(core_vec_double_t, core_vec_double, double, void, TRIVIAL_COMP)
