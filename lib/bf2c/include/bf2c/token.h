#ifndef BF2C_TOKEN_H_
#define BF2C_TOKEN_H_

typedef enum token_type_t {
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
} token_type_t;

token_type_t bf2c_token_from_char(char source);
char bf2c_token_to_char(token_type_t token);


#endif /* ifndef BF2C_TOKEN_H_ */
