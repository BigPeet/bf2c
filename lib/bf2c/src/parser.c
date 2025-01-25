#include "bf2c/parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bf2c/program.h"
#include "bf2c/token.h"

#include "core/abort.h"
#include "core/logging.h"

// declare constant as enum
enum
{
    BUFFER_SIZE = 1024
};

// FILE* -> char const*
// char const* -> token*
// token* -> command*
// command* -> program*
static token_type* bf2c_parse_tokens_from_file(FILE* file)
{
    // TODO: move outside of function and add helper functions to
    //       manage the linked list (add, free)
    typedef struct token_list
    {
        token_type token;
        struct token_list* next;
    } token_list;

    char buffer[BUFFER_SIZE];
    token_list* head = NULL;
    token_list* cur  = NULL;
    size_t token_cnt = 0;
    while (!feof(file))
    {
        size_t const ret_val = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, file);
        for (size_t i = 0; i < ret_val; ++i)
        {
            token_type token = bf2c_token_from_char(buffer[i]);
            if (token != TOKEN_COMMENT)
            {
                token_list* new_token = malloc(sizeof(token_list));
                ABORT_IF(!new_token);
                new_token->token = token;
                new_token->next  = NULL;
                ++token_cnt;
                if (head == NULL)
                {
                    head = new_token;
                    cur  = head;
                }
                else
                {
                    cur->next = new_token;
                    cur       = new_token;
                }
            }
            // Use a linked list to store tokens during parsing
            // count the number of tokens to allocate the correct amount of memory
            // return the linked list as an array
        }
        if (ferror(file))
        {
            LOG_ERROR_MSG("Error reading file");
            return NULL;
        }
    }
    cur = head;
    printf("Parsed %zu tokens: ", token_cnt);
    while (cur != NULL)
    {
        printf("%c", bf2c_token_to_char(cur->token));
        cur = cur->next;
    }
    printf("\n");
    return NULL;
}

// OR return pointer
static program_t bf2c_parse_program(token_type const* tokens)
{
    // OR this function takes ownership of the tokens and frees them
    (void) tokens;
    /*return NULL;*/
    return (program_t){0};
}

// OR return a result
program_t bf2c_parse_file(FILE* file)
{
    token_type* tokens = bf2c_parse_tokens_from_file(file);
    program_t program  = bf2c_parse_program(tokens);

    free(tokens);
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
