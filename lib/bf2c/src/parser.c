#include "bf2c/parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bf2c/program.h"
#include "bf2c/token.h"

#include "core/abort.h"
#include "core/logging.h"
#include "core/vector.h"

// declare constant as enum
enum
{
    BUFFER_SIZE = 1024
};

VECTOR_DECLARE_AND_DEFINE_WITH_PREFIX(token_vec_t, token_vec, token_type, void)

// FILE* -> char const*
// char const* -> token*
// token* -> command*
// command* -> program*
static token_vec_t bf2c_parse_tokens_from_file(FILE* file)
{
    token_vec_t tokens = token_vec_create();

    char buffer[BUFFER_SIZE];
    while (!feof(file))
    {
        size_t const ret_val = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, file);
        for (size_t i = 0; i < ret_val; ++i)
        {
            token_type token = bf2c_token_from_char(buffer[i]);
            if (token != TOKEN_COMMENT)
            {
                token_vec_push_back(&tokens, token);
            }
        }
        if (ferror(file))
        {
            LOG_ERROR_MSG("Error reading file");
            // OR Abort
            // OR clear vector and return
            token_vec_destroy(&tokens);
            return token_vec_create();
        }
    }
    printf("Parsed %zu tokens: ", tokens.size);
    VEC_FOR_EACH(token_type, tok, tokens)
    {
        printf("%c", bf2c_token_to_char(tok));
    }
    printf("\n");
    return tokens;
}

// OR return pointer
static program_t bf2c_parse_program(token_vec_t const* tokens)
{
    // OR this function takes ownership of the tokens and frees them
    VEC_FOR_EACH(token_type, tok, *tokens)
    {
        LOG_DEBUG("Parsing token: %c", bf2c_token_to_char(tok));
        switch (tok) {
          case TOKEN_PLUS:
            break;
          default:
            break;
        }
    }
    return (program_t){0};
}

// OR return a result
program_t bf2c_parse_file(FILE* file)
{
    token_vec_t tokens = bf2c_parse_tokens_from_file(file);
    program_t program  = bf2c_parse_program(&tokens);
    token_vec_destroy(&tokens);
    return program;
}

// TODO: add result for program_t with parser errors as error type
// OR return a result
program_t bf2c_parse_filename(char const* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        // TODO: return a result with error
        return (program_t){0};
    }
    LOG_DEBUG("Parsing file: %s", filename);
    program_t const program = bf2c_parse_file(file);
    (void) fclose(file);
    return program;
}
