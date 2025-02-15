#include "bf2c/token.h"

token_type_t bf2c_token_from_char(char source)
{
    switch (source)
    {
        case '+':
            return TOKEN_PLUS;
        case '-':
            return TOKEN_MINUS;
        case '<':
            return TOKEN_LEFT;
        case '>':
            return TOKEN_RIGHT;
        case '[':
            return TOKEN_LBRACKET;
        case ']':
            return TOKEN_RBRACKET;
        case '.':
            return TOKEN_PERIOD;
        case ',':
            return TOKEN_COMMA;
        case '#':
            return TOKEN_POUND;
        case '!':
            return TOKEN_EXCLAMATION;
        default:
            return TOKEN_COMMENT;
    }
}

char bf2c_token_to_char(token_type_t token)
{
    switch (token)
    {
        case TOKEN_PLUS:
            return '+';
        case TOKEN_MINUS:
            return '-';
        case TOKEN_LEFT:
            return '<';
        case TOKEN_RIGHT:
            return '>';
        case TOKEN_LBRACKET:
            return '[';
        case TOKEN_RBRACKET:
            return ']';
        case TOKEN_PERIOD:
            return '.';
        case TOKEN_COMMA:
            return ',';
        case TOKEN_POUND:
            return '#';
        case TOKEN_EXCLAMATION:
            return '!';
        case TOKEN_COMMENT:
            // null character, not printed by printf but may cause issues when used inside a string
            return '\0';
    }
    return '\0'; // should be unreachable
}
