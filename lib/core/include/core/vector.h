#ifndef CORE_VECTOR_H_
#define CORE_VECTOR_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "core/abort.h"

#define INTERNAL_FOR_EACH_SETUP(vector)                                                     \
    for (size_t keep = 1, vector##_iterator = 0; vector##_iterator < (vector).size && keep; \
         vector##_iterator++, keep          = !keep)

// NOLINTBEGIN(bugprone-macro-parentheses)
#define VEC_FOR_EACH(value_type, elem, vector) \
    INTERNAL_FOR_EACH_SETUP(vector)            \
    for (value_type elem = (vector).data[vector##_iterator]; keep; keep = !keep)

#define VEC_FOR_EACH_REF(value_type, elem, vector) \
    INTERNAL_FOR_EACH_SETUP(vector)                \
    for (value_type* elem = (vector).data + vector##_iterator; keep; keep = !keep)
// NOLINTEND(bugprone-macro-parentheses)

// TODO: Should "setters" take a pointer to the value?
// E.g. void vector_push_back(vector_t* vector, value_t const* value);
// TODO: Should there be a "contains" function for pointer equality (same object, not same value)?
// TODO: Should clear use memset or just set size to 0?
// TODO: Add "extend" functions
// TODO: Add "result-based" functions
#define INTERNAL_VECTOR_DECLARE_FUNCTIONS(type_name, prefix, value_type, result_type) \
    type_name prefix##_create(void);                                                  \
    type_name prefix##_with_capacity(size_t capacity);                                \
    type_name prefix##_from(value_type const* array, size_t len);                     \
    void prefix##_destroy(type_name* vector);                                         \
    void prefix##_clear(type_name* vector);                                           \
    bool prefix##_is_empty(type_name const* vector);                                  \
    bool prefix##_contains(type_name const* vector, value_type value);                \
    size_t prefix##_find(type_name const* vector, value_type value);                  \
    void prefix##_remove_if(type_name* vector, prefix##_predicate pred);              \
    void prefix##_reserve(type_name* vector, size_t new_capacity);                    \
    void prefix##_shrink_to_fit(type_name* vector);                                   \
    void prefix##_push_front(type_name* vector, value_type value);                    \
    void prefix##_push_back(type_name* vector, value_type value);                     \
    value_type prefix##_pop_front(type_name* vector);                                 \
    value_type prefix##_pop_back(type_name* vector);                                  \
    value_type* prefix##_get(type_name* vector, size_t index);                        \
    value_type const* prefix##_getc(type_name const* vector, size_t index);           \
    void prefix##_remove(type_name* vector, size_t index);                            \
    void prefix##_insert(type_name* vector, size_t index, value_type value);

#define VECTOR_DECLARE_WITH_PREFIX(type_name, prefix, value_type, result_type) \
    typedef struct type_name                                                   \
    {                                                                          \
        value_type* data;                                                      \
        size_t size;                                                           \
        size_t capacity;                                                       \
    } type_name;                                                               \
    typedef bool (*prefix##_predicate)(value_type const* value);               \
    INTERNAL_VECTOR_DECLARE_FUNCTIONS(type_name, prefix, value_type, result_type)

#define VECTOR_DECLARE(type_name, value_type, result_type) \
    VECTOR_DECLARE_WITH_PREFIX(type_name, type_name, value_type, result_type)

// TODO: add implementations
#define INTERNAL_VECTOR_DEFINE_FUNCTIONS(type_name, prefix, value_type, result_type)        \
    type_name prefix##_create(void)                                                         \
    {                                                                                       \
        return (type_name){0};                                                              \
    }                                                                                       \
    type_name prefix##_with_capacity(size_t capacity)                                       \
    {                                                                                       \
        type_name vector = prefix##_create();                                               \
        prefix##_reserve(&vector, capacity);                                                \
        return vector;                                                                      \
    }                                                                                       \
    type_name prefix##_from(value_type const* array, size_t len)                            \
    {                                                                                       \
        LOG_MSG_AND_ABORT_IF(!array,                                                        \
                             "Given array is NULL. Use prefix##_with_capacity() instead."); \
        type_name vector = prefix##_with_capacity(len);                                     \
        memcpy(vector.data, array, len * sizeof(value_type));                               \
        vector.size = len;                                                                  \
        return vector;                                                                      \
    }                                                                                       \
    void prefix##_destroy(type_name* vector)                                                \
    {                                                                                       \
        if (vector)                                                                         \
        {                                                                                   \
            free(vector->data);                                                             \
            vector->data     = NULL;                                                        \
            vector->size     = 0;                                                           \
            vector->capacity = 0;                                                           \
        }                                                                                   \
    }                                                                                       \
    void prefix##_clear(type_name* vector)                                                  \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        if (vector->size > 0)                                                               \
        {                                                                                   \
            memset(vector->data, 0, vector->size * sizeof(value_type));                     \
            vector->size = 0;                                                               \
        }                                                                                   \
    }                                                                                       \
    bool prefix##_is_empty(type_name const* vector)                                         \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        return vector->size == 0;                                                           \
    }                                                                                       \
    bool prefix##_contains(type_name const* vector, value_type value)                       \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        for (size_t i = 0; i < vector->size; i++)                                           \
        {                                                                                   \
            if (vector->data[i] == value)                                                   \
            {                                                                               \
                return true;                                                                \
            }                                                                               \
        }                                                                                   \
        return false;                                                                       \
    }                                                                                       \
    size_t prefix##_find(type_name const* vector, value_type value)                         \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        for (size_t i = 0; i < vector->size; i++)                                           \
        {                                                                                   \
            if (vector->data[i] == value)                                                   \
            {                                                                               \
                return i;                                                                   \
            }                                                                               \
        }                                                                                   \
        return (size_t) - 1;                                                                \
    }                                                                                       \
    void prefix##_remove_if(type_name* vector, prefix##_predicate pred)                     \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        for (size_t i = 0; i < vector->size; i++)                                           \
        {                                                                                   \
            if (pred(&vector->data[i]))                                                     \
            {                                                                               \
                prefix##_remove(vector, i);                                                 \
                i--;                                                                        \
            }                                                                               \
        }                                                                                   \
    }                                                                                       \
    void prefix##_reserve(type_name* vector, size_t new_capacity)                           \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        if (new_capacity <= vector->capacity)                                               \
        {                                                                                   \
            return;                                                                         \
        }                                                                                   \
        value_type* allocated = realloc(vector->data, new_capacity * sizeof(value_type));   \
        LOG_MSG_AND_ABORT_IF(!allocated, "Failed to allocate memory for vector.");          \
        vector->data     = allocated;                                                       \
        vector->capacity = new_capacity;                                                    \
    }                                                                                       \
    void prefix##_shrink_to_fit(type_name* vector)                                          \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        if (vector->size == vector->capacity)                                               \
        {                                                                                   \
            return;                                                                         \
        }                                                                                   \
        if (vector->size == 0)                                                              \
        {                                                                                   \
            free(vector->data);                                                             \
            vector->data     = NULL;                                                        \
            vector->capacity = 0;                                                           \
            return;                                                                         \
        }                                                                                   \
        value_type* allocated = realloc(vector->data, vector->size * sizeof(value_type));   \
        LOG_MSG_AND_ABORT_IF(!allocated, "Failed to allocate memory for vector.");          \
        vector->data     = allocated;                                                       \
        vector->capacity = vector->size;                                                    \
    }                                                                                       \
    static void prefix##_grow(type_name* vector)                                            \
    {                                                                                       \
        assert(vector);                                                                     \
        size_t new_capacity = vector->capacity == 0 ? 1 : vector->capacity * 2;             \
        prefix##_reserve(vector, new_capacity);                                             \
    }                                                                                       \
    void prefix##_push_front(type_name* vector, value_type value)                           \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        prefix##_insert(vector, 0, value);                                                  \
    }                                                                                       \
    void prefix##_push_back(type_name* vector, value_type value)                            \
    {                                                                                       \
        ABORT_IF(!vector);                                                                  \
        prefix##_insert(vector, vector->size, value);                                       \
    }                                                                                       \
    value_type prefix##_pop_front(type_name* vector)                                        \
    {                                                                                       \
        ABORT_IF(!vector || vector->size == 0);                                             \
        value_type value = vector->data[0];                                                 \
        prefix##_remove(vector, 0);                                                         \
        return value;                                                                       \
    }                                                                                       \
    value_type prefix##_pop_back(type_name* vector)                                         \
    {                                                                                       \
        ABORT_IF(!vector || vector->size == 0);                                             \
        value_type value = vector->data[vector->size - 1];                                  \
        prefix##_remove(vector, vector->size - 1);                                          \
        return value;                                                                       \
    }                                                                                       \
    value_type* prefix##_get(type_name* vector, size_t index)                               \
    {                                                                                       \
        ABORT_IF(!vector || index >= vector->size);                                         \
        return &vector->data[index];                                                        \
    }                                                                                       \
    value_type const* prefix##_getc(type_name const* vector, size_t index)                  \
    {                                                                                       \
        ABORT_IF(!vector || index >= vector->size);                                         \
        return &vector->data[index];                                                        \
    }                                                                                       \
    void prefix##_remove(type_name* vector, size_t index)                                   \
    {                                                                                       \
        ABORT_IF(!vector || index >= vector->size);                                         \
        for (size_t i = index + 1; i < vector->size; i++)                                   \
        {                                                                                   \
            vector->data[i - 1] = vector->data[i];                                          \
        }                                                                                   \
        vector->size--;                                                                     \
    }                                                                                       \
    void prefix##_insert(type_name* vector, size_t index, value_type value)                 \
    {                                                                                       \
        ABORT_IF(!vector || index > vector->size);                                          \
        if (vector->size == vector->capacity)                                               \
        {                                                                                   \
            prefix##_grow(vector);                                                          \
        }                                                                                   \
        for (size_t i = vector->size; i > index; i--)                                       \
        {                                                                                   \
            vector->data[i] = vector->data[i - 1];                                          \
        }                                                                                   \
        vector->data[index] = value;                                                        \
        vector->size++;                                                                     \
    }

#define VECTOR_DEFINE_WITH_PREFIX(type_name, prefix, value_type, result_type) \
    INTERNAL_VECTOR_DEFINE_FUNCTIONS(type_name, prefix, value_type, result_type)

#define VECTOR_DEFINE(type_name, value_type, result_type) \
    VECTOR_DEFINE_WITH_PREFIX(type_name, type_name, value_type, result_type)

#define VECTOR_DECLARE_AND_DEFINE_WITH_PREFIX(type_name, prefix, value_type, result_type) \
    VECTOR_DECLARE_WITH_PREFIX(type_name, prefix, value_type, result_type)                \
    VECTOR_DEFINE_WITH_PREFIX(type_name, prefix, value_type, result_type)

#define VECTOR_DECLARE_AND_DEFINE(type_name, value_type, result_type) \
    VECTOR_DECLARE(type_name, value_type, result_type)                \
    VECTOR_DEFINE(type_name, value_type, result_type)

#ifdef CORE_VECTOR_DECLARE_BASIC_TYPES

// TODO: Add more basic types as needed
VECTOR_DECLARE_WITH_PREFIX(core_vec_int_t, core_vec_int, int32_t, void)
VECTOR_DECLARE_WITH_PREFIX(core_vec_char_t, core_vec_char, char, void)
VECTOR_DECLARE_WITH_PREFIX(core_vec_string_t, core_vec_string, char const*, void)
VECTOR_DECLARE_WITH_PREFIX(core_vec_double_t, core_vec_double, double, void)

#endif /* ifdef CORE_VECTOR_DECLARE_BASIC_TYPES */
#endif /* ifndef CORE_VECTOR_H_ */
