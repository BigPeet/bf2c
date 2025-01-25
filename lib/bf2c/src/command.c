#include "bf2c/command.h"
#include "bf2c/token.h"
#include "core/vector.h"

#define COMMAND_CMP(a, b) TRIVIAL_COMP(a.type, b.type)
VECTOR_DEFINE_WITH_PREFIX(command_vec_t, command_vec, command_t, void, COMMAND_CMP)

command_type bf2c_command_from_token(token_type token)
{
    switch (token)
    {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return COMMAND_TYPE_CHANGE_VAL;
        case TOKEN_LEFT:
        case TOKEN_RIGHT:
            return COMMAND_TYPE_CHANGE_PTR;
        case TOKEN_LBRACKET:
            return COMMAND_TYPE_LOOP_START;
        case TOKEN_RBRACKET:
            return COMMAND_TYPE_LOOP_END;
        case TOKEN_PERIOD:
            return COMMAND_TYPE_OUT;
        case TOKEN_COMMA:
            return COMMAND_TYPE_IN;
        case TOKEN_POUND:
            return COMMAND_TYPE_DEBUG;
        case TOKEN_EXCLAMATION:
        case TOKEN_COMMENT:
            return COMMAND_TYPE_UNKNOWN;
    }
}

int32_t bf2c_command_value(token_type token)
{
    switch (token)
    {
        case TOKEN_PLUS:
        case TOKEN_RIGHT:
            return 1;
        case TOKEN_MINUS:
        case TOKEN_LEFT:
            return -1;
        default:
            return 0;
    }
}

char const* bf2c_command_type_to_string(command_type type)
{
    switch (type)
    {
        case COMMAND_TYPE_CHANGE_VAL:
            return "CHANGE_VAL";
        case COMMAND_TYPE_CHANGE_PTR:
            return "CHANGE_PTR";
        case COMMAND_TYPE_OUT:
            return "OUT";
        case COMMAND_TYPE_IN:
            return "IN";
        case COMMAND_TYPE_LOOP_START:
            return "LOOP_START";
        case COMMAND_TYPE_LOOP_END:
            return "LOOP_END";
        case COMMAND_TYPE_DEBUG:
            return "DEBUG";
        case COMMAND_TYPE_UNKNOWN:
            return "UNKNOWN";
    }
}
