#ifndef BF2C_TOKEN_H_
#define BF2C_TOKEN_H_

typedef enum token_type
{
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_LEFT,
    TOKEN_RIGHT,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_PERIOD,
    TOKEN_COMMA,
    TOKEN_POUND,
    TOKEN_EXCLAMATION,
    TOKEN_COMMENT,
} token_type;

token_type bf2c_token_from_char(char source);


#endif /* ifndef BF2C_TOKEN_H_ */
