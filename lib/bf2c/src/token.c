#include "bf2c/token.h"

token_type bf2c_token_from_char(char source)
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
