#include "bf2c/parser.h"

#include <stdlib.h>

#include "bf2c/program.h"
#include "bf2c/token.h"

// FILE* -> char const*
// char const* -> token*
// token* -> command*
// command* -> program*
static token_type* bf2c_parse_tokens_from_file(FILE* file)
{
    (void) file;
    return NULL;
}

// OR return pointer
static program_t bf2c_parse_program(token_type const* tokens)
{
    // OR this function takes ownership of the tokens and frees them
    (void) tokens;
    /*return NULL;*/
    return (program_t) {0};
}

program_t bf2c_parse_file(FILE* file)
{
    token_type* tokens = bf2c_parse_tokens_from_file(file);
    program_t program = bf2c_parse_program(tokens);

    free(tokens);
    return program;
}
