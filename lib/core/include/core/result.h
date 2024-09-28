#ifndef CORE_RESULT_H_
#define CORE_RESULT_H_

#include <stdbool.h>
#include "core/abort.h"

#define INTERNAL_RESULT_DECLARE_VALUE_FUNCTIONS(type_name, value_type, error_type)       \
    type_name type_name##_create_from_value(value_type value);                           \
    void type_name##_set_value(type_name* result, value_type value);                     \
    value_type const* type_name##_get_value(type_name const* result);                    \
    value_type* type_name##_get_value_mut(type_name* result);                            \
    value_type type_name##_unwrap(type_name const* result);                              \
    value_type type_name##_unwrap_or(type_name const* result, value_type default_value); \
    value_type type_name##_unwrap_or_default(type_name const* result);                   \
    value_type type_name##_unwrap_or_else(type_name const* result,                       \
                                          value_type (*default_fn)(void));               \
    bool type_name##_unwrap_or_handle(type_name const* result,                           \
                                      value_type* out,                                   \
                                      void (*handler)(error_type));                      \
    bool type_name##_try_unwrap(type_name const* result, value_type* out);               \
    value_type type_name##_expect(type_name const* result, char const* message);


#define INTERNAL_RESULT_DECLARE_ERROR_FUNCTIONS(type_name, value_type, error_type)           \
    type_name type_name##_create_from_error(error_type error);                               \
    void type_name##_set_error(type_name* result, error_type error);                         \
    error_type const* type_name##_get_error(type_name const* result);                        \
    error_type* type_name##_get_error_mut(type_name* result);                                \
    error_type type_name##_unwrap_err(type_name const* result);                              \
    error_type type_name##_unwrap_err_or(type_name const* result, error_type default_value); \
    error_type type_name##_unwrap_err_or_default(type_name const* result);                   \
    error_type type_name##_unwrap_err_or_else(type_name const* result,                       \
                                              error_type (*default_fn)(void));               \
    bool type_name##_unwrap_err_or_handle(type_name const* result,                           \
                                          error_type* out,                                   \
                                          void (*handler)(value_type));                      \
    bool type_name##_try_unwrap_err(type_name const* result, error_type* out);               \
    error_type type_name##_expect_err(type_name const* result, char const* message);


#define INTERNAL_RESULT_DEFINE_VALUE_FUNCTIONS(type_name, value_type, error_type)                  \
    type_name type_name##_create_from_value(value_type value)                                      \
    {                                                                                              \
        return (type_name){.storage.value = value, .has_value = true};                             \
    }                                                                                              \
    void type_name##_set_value(type_name* result, value_type value)                                \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        result->storage.value = value;                                                             \
        result->has_value     = true;                                                              \
    }                                                                                              \
    value_type const* type_name##_get_value(type_name const* result)                               \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        return result->has_value ? &result->storage.value : NULL;                                  \
    }                                                                                              \
    value_type* type_name##_get_value_mut(type_name* result)                                       \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        return result->has_value ? &result->storage.value : NULL;                                  \
    }                                                                                              \
    value_type type_name##_unwrap(type_name const* result)                                         \
    {                                                                                              \
        ABORT_IF(!result || !result->has_value);                                                   \
        return result->storage.value;                                                              \
    }                                                                                              \
    value_type type_name##_unwrap_or(type_name const* result, value_type default_value)            \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        return result->has_value ? result->storage.value : default_value;                          \
    }                                                                                              \
    value_type type_name##_unwrap_or_default(type_name const* result)                              \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        if (result->has_value)                                                                     \
        {                                                                                          \
            return result->storage.value;                                                          \
        }                                                                                          \
        value_type const default_value = {0};                                                      \
        return default_value;                                                                      \
    }                                                                                              \
    value_type type_name##_unwrap_or_else(type_name const* result, value_type (*default_fn)(void)) \
    {                                                                                              \
        ABORT_IF(!result || !default_fn);                                                          \
        return result->has_value ? result->storage.value : default_fn();                           \
    }                                                                                              \
    bool type_name##_unwrap_or_handle(type_name const* result,                                     \
                                      value_type* out,                                             \
                                      void (*handler)(error_type))                                 \
    {                                                                                              \
        ABORT_IF(!result || !out);                                                                 \
        if (result->has_value)                                                                     \
        {                                                                                          \
            *out = result->storage.value;                                                          \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            handler(result->storage.error);                                                        \
        }                                                                                          \
        return result->has_value;                                                                  \
    }                                                                                              \
    bool type_name##_try_unwrap(type_name const* result, value_type* out)                          \
    {                                                                                              \
        ABORT_IF(!result || !out);                                                                 \
        if (result->has_value)                                                                     \
        {                                                                                          \
            *out = result->storage.value;                                                          \
        }                                                                                          \
        return result->has_value;                                                                  \
    }                                                                                              \
    value_type type_name##_expect(type_name const* result, char const* message)                    \
    {                                                                                              \
        ABORT_IF(!result);                                                                         \
        LOG_AND_ABORT_IF(!result->has_value, message);                                             \
        return result->storage.value;                                                              \
    }


#define INTERNAL_RESULT_DEFINE_ERROR_FUNCTIONS(type_name, value_type, error_type)           \
    type_name type_name##_create_from_error(error_type error)                               \
    {                                                                                       \
        return (type_name){.storage.error = error, .has_value = false};                     \
    }                                                                                       \
    void type_name##_set_error(type_name* result, error_type error)                         \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        result->storage.error = error;                                                      \
        result->has_value     = false;                                                      \
    }                                                                                       \
    error_type const* type_name##_get_error(type_name const* result)                        \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        return result->has_value ? NULL : &result->storage.error;                           \
    }                                                                                       \
    error_type* type_name##_get_error_mut(type_name* result)                                \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        return result->has_value ? NULL : &result->storage.error;                           \
    }                                                                                       \
    error_type type_name##_unwrap_err(type_name const* result)                              \
    {                                                                                       \
        ABORT_IF(!result || result->has_value);                                             \
        return result->storage.error;                                                       \
    }                                                                                       \
    error_type type_name##_unwrap_err_or(type_name const* result, error_type default_value) \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        return result->has_value ? default_value : result->storage.error;                   \
    }                                                                                       \
    error_type type_name##_unwrap_err_or_default(type_name const* result)                   \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        if (!result->has_value)                                                             \
        {                                                                                   \
            return result->storage.error;                                                   \
        }                                                                                   \
        error_type const default_value = {0};                                               \
        return default_value;                                                               \
    }                                                                                       \
    error_type type_name##_unwrap_err_or_else(type_name const* result,                      \
                                              error_type (*default_fn)(void))               \
    {                                                                                       \
        ABORT_IF(!result || !default_fn);                                                   \
        return result->has_value ? default_fn() : result->storage.error;                    \
    }                                                                                       \
    bool type_name##_try_unwrap_err(type_name const* result, error_type* out)               \
    {                                                                                       \
        ABORT_IF(!result || !out);                                                          \
        if (!result->has_value)                                                             \
        {                                                                                   \
            *out = result->storage.error;                                                   \
        }                                                                                   \
        return !result->has_value;                                                          \
    }                                                                                       \
    error_type type_name##_expect_err(type_name const* result, char const* message)         \
    {                                                                                       \
        ABORT_IF(!result);                                                                  \
        LOG_AND_ABORT_IF(result->has_value, message);                                       \
        return result->storage.error;                                                       \
    }

#define RESULT_DECLARE(type_name, value_type, error_type)                      \
    typedef struct type_name                                                   \
    {                                                                          \
        union                                                                  \
        {                                                                      \
            value_type value;                                                  \
            error_type error;                                                  \
        } storage;                                                             \
        bool has_value;                                                        \
    } type_name;                                                               \
                                                                               \
    bool type_name##_has_value(type_name const* result);                       \
                                                                               \
    INTERNAL_RESULT_DECLARE_VALUE_FUNCTIONS(type_name, value_type, error_type) \
    INTERNAL_RESULT_DECLARE_ERROR_FUNCTIONS(type_name, value_type, error_type)


#define RESULT_DEFINE(type_name, value_type, error_type)                      \
    bool type_name##_has_value(type_name const* result)                       \
    {                                                                         \
        ABORT_IF(!result);                                                    \
        return result->has_value;                                             \
    }                                                                         \
    bool type_name##_unwrap_err_or_handle(type_name const* result,            \
                                          error_type* out,                    \
                                          void (*handler)(value_type))        \
    {                                                                         \
        ABORT_IF(!result || !out);                                            \
        if (!result->has_value)                                               \
        {                                                                     \
            *out = result->storage.error;                                     \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            handler(result->storage.value);                                   \
        }                                                                     \
        return !result->has_value;                                            \
    }                                                                         \
    INTERNAL_RESULT_DEFINE_VALUE_FUNCTIONS(type_name, value_type, error_type) \
    INTERNAL_RESULT_DEFINE_ERROR_FUNCTIONS(type_name, value_type, error_type)


#define RESULT_VOID_DECLARE(type_name, error_type)       \
    typedef struct type_name                             \
    {                                                    \
        union                                            \
        {                                                \
            error_type error;                            \
        } storage;                                       \
        bool has_value;                                  \
    } type_name;                                         \
                                                         \
    type_name type_name##_create_from_value(void);       \
    void type_name##_set_value(type_name* result);       \
    bool type_name##_has_value(type_name const* result); \
                                                         \
    INTERNAL_RESULT_DECLARE_ERROR_FUNCTIONS(type_name, void, error_type)


#define RESULT_VOID_DEFINE(type_name, error_type)                  \
    type_name type_name##_create_from_value(void)                  \
    {                                                              \
        return (type_name){.has_value = true};                     \
    }                                                              \
    void type_name##_set_value(type_name* result)                  \
    {                                                              \
        ABORT_IF(!result);                                         \
        result->has_value = true;                                  \
    }                                                              \
    bool type_name##_has_value(type_name const* result)            \
    {                                                              \
        ABORT_IF(!result);                                         \
        return result->has_value;                                  \
    }                                                              \
    bool type_name##_unwrap_err_or_handle(type_name const* result, \
                                          error_type* out,         \
                                          void (*handler)(void))   \
    {                                                              \
        ABORT_IF(!result || !out);                                 \
        if (!result->has_value)                                    \
        {                                                          \
            *out = result->storage.error;                          \
        }                                                          \
        else                                                       \
        {                                                          \
            handler();                                             \
        }                                                          \
        return !result->has_value;                                 \
    }                                                              \
    INTERNAL_RESULT_DEFINE_ERROR_FUNCTIONS(type_name, void, error_type)


#define RESULT_DECLARE_AND_DEFINE(type_name, value_type, error_type) \
    RESULT_DECLARE(type_name, value_type, error_type)                \
    RESULT_DEFINE(type_name, value_type, error_type)


#define RESULT_VOID_DECLARE_AND_DEFINE(type_name, error_type) \
    RESULT_VOID_DECLARE(type_name, error_type)                \
    RESULT_VOID_DEFINE(type_name, error_type)

#endif /* ifndef CORE_RESULT_H_ */
