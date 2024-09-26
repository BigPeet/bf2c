#include "core/result.h"
#include "core/abort.h"
#include "core/logging.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// example to test API

typedef struct test_result_t
{
    union
    {
        double value;
        int error;
    } content;
    bool has_value;
} test_result_t;

test_result_t test_result_create_from_value(double value)
{
    return (test_result_t){.content.value = value, .has_value = true};
}

test_result_t test_result_create_from_error(int error)
{
    return (test_result_t){.content.error = error, .has_value = false};
}

void test_result_set_value(test_result_t* result, double value)
{
    assert(result);
    result->content.value = value;
    result->has_value     = true;
}

void test_result_set_error(test_result_t* result, int error)
{
    assert(result);
    result->content.error = error;
    result->has_value     = false;
}

bool test_result_has_value(test_result_t const* result)
{
    return result->has_value;
}

double* test_result_get_value(test_result_t* result)
{
    assert(result);
    return result->has_value ? &result->content.value : NULL;
}

int* test_result_get_error(test_result_t* result)
{
    assert(result);
    return result->has_value ? NULL : &result->content.error;
}

double test_result_unwrap(test_result_t const* result)
{
    assert(result);
    ABORT_IF(!result->has_value);
    return result->content.value;
}

double test_result_unwrap_or(test_result_t const* result, double default_value)
{
    assert(result);
    return result->has_value ? result->content.value : default_value;
}

double test_result_unwrap_or_default(test_result_t const* result)
{
    assert(result);
    if (result->has_value)
    {
        return result->content.value;
    }
    double const default_value = {0};
    return default_value;
}

double test_result_unwrap_or_else(test_result_t const* result, double (*default_fn)(void))
{
    assert(result);
    assert(default_fn);
    return result->has_value ? result->content.value : default_fn();
}

double test_result_expect(test_result_t const* result, char const* message)
{
    assert(result);
    LOG_AND_ABORT_IF(!result->has_value, message);
    return result->content.value;
}

int test_result_unwrap_err(test_result_t const* result)
{
    assert(result);
    ABORT_IF(result->has_value);
    return result->content.error;
}

int test_result_unwrap_err_or(test_result_t const* result, int default_value)
{
    assert(result);
    return !result->has_value ? result->content.error : default_value;
}

int test_result_unwrap_err_or_default(test_result_t const* result)
{
    assert(result);
    if (!result->has_value)
    {
        return result->content.error;
    }
    int const default_value = {0};
    return default_value;
}

int test_result_unwrap_err_or_else(test_result_t const* result, int (*default_fn)(void))
{
    assert(result);
    assert(default_fn);
    return !result->has_value ? result->content.error : default_fn();
}

int test_result_expect_err(test_result_t const* result, char const* message)
{
    assert(result);
    LOG_AND_ABORT_IF(result->has_value, message);
    return result->content.error;
}

int main(int argc, char* argv[])
{
    LOGGING_INIT(LOGLEVEL_INFO);
    test_result_t res = test_result_create_from_error(2);
    /*test_result_t res = test_result_create_from_value(42.0);*/
    double val = test_result_unwrap(&res);
    /*double val = test_result_expect(&res, "There was no value");*/
    /*double* val = test_result_get_value(&res);*/
    return 0;
}
