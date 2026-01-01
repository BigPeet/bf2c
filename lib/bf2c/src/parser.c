#include "bf2c/parser.h"

#include <stdint.h>
#include <stdio.h>

#include "bf2c/command.h"
#include "bf2c/program.h"
#include "bf2c/token.h"
#include "core/abort.h"
#include "core/logging.h"
#include "core/vector.h"

// declare constant as enum
enum {
    BUFFER_SIZE = 1024
};

VECTOR_DECLARE_AND_DEFINE_WITH_PREFIX(token_vec_t, token_vec, token_type_t, void, TRIVIAL_COMP)

static token_vec_t bf2c_parse_tokens_from_file(FILE* file) {
    token_vec_t tokens = token_vec_create();
    if (!file) {
        // no input, return empty vector
        return tokens;
    }

    char buffer[BUFFER_SIZE];
    while (!feof(file)) {
        size_t const ret_val = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, file);
        for (size_t i = 0; i < ret_val; ++i) {
            token_type_t token = bf2c_token_from_char(buffer[i]);
            if (token != TOKEN_COMMENT) {
                token_vec_push_back(&tokens, token);
            }
        }
        if (ferror(file)) {
            LOG_ERROR_MSG("Error reading file");
            // OR Abort
            // OR clear vector and return
            token_vec_destroy(&tokens);
            return token_vec_create();
        }
    }
    return tokens;
}

static token_vec_t bf2c_parse_tokens_from_text(char const* text) {
    token_vec_t tokens = token_vec_create();
    if (!text) {
        // no input, return empty vector
        return tokens;
    }

    char cur = 0;
    while ((cur = *text++)) {
        token_type_t token = bf2c_token_from_char(cur);
        if (token != TOKEN_COMMENT) {
            token_vec_push_back(&tokens, token);
        }
    }
    return tokens;
}

// OR return pointer
static program_t bf2c_parse_program(token_vec_t const* tokens) {
    // OR this function takes ownership of the tokens and frees them
    command_vec_t commands = command_vec_create();

    size_t idx = 0;
    while (idx < tokens->size) {
        token_type_t tok   = tokens->data[idx];
        command_type_t cur = bf2c_command_from_token(tok);
        if (cur == COMMAND_TYPE_CHANGE_PTR || cur == COMMAND_TYPE_CHANGE_VAL) {
            // Match streaks of "additive" commands
            int32_t value = bf2c_command_value(tok);
            while (idx < tokens->size - 1 && bf2c_command_from_token(tokens->data[idx + 1]) == cur)
            {
                ++idx;
                value += bf2c_command_value(tokens->data[idx]);
            }
            if (value != 0) {
                command_vec_push_back(&commands, (command_t){value, cur});
            }
        } else {
            // Match non-"additive" commands
            command_vec_push_back(&commands, (command_t){0, cur});
        }
        ++idx;
    }

    // Match loops
    core_vec_size_t stack = core_vec_size_create();
    for (size_t i = 0; i < commands.size; ++i) {
        command_type_t cur = commands.data[i].type;
        if (cur == COMMAND_TYPE_LOOP_START) {
            core_vec_size_push_back(&stack, i);
        } else if (cur == COMMAND_TYPE_LOOP_END) {
            // TODO: return a result/error instead of aborting
            LOG_AND_ABORT_IF(core_vec_size_is_empty(&stack), "Unmatched loop end at index %zu", i);
            size_t start               = core_vec_size_pop_back(&stack);
            int32_t const delta        = (int32_t) (i - start); // FIXME: potential data loss
            commands.data[start].value = delta;
            commands.data[i].value     = -delta;
        }
    }
    // TODO: return a result/error instead of aborting
    // TODO: can we match this back to the original source or at least token?
    LOG_AND_ABORT_IF(!core_vec_size_is_empty(&stack),
                     "Unmatched loop start at index %zu",
                     stack.data[0]);
    core_vec_size_destroy(&stack);
    return bf2c_program_create(commands);
}

// OR return a result
program_t bf2c_parse_file(FILE* file) {
    token_vec_t tokens = bf2c_parse_tokens_from_file(file);
    program_t program  = bf2c_parse_program(&tokens);
    token_vec_destroy(&tokens);
    return program;
}

// TODO: add result for program_t with parser errors as error type
// OR return a result
program_t bf2c_parse_file_by_name(char const* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        // TODO: return a result with error
        return (program_t){0};
    }
    LOG_DEBUG("Parsing file: %s", filename);
    program_t const program = bf2c_parse_file(file);
    (void) fclose(file);
    return program;
}

// TODO: add result for program_t with parser errors as error type
// OR return a result
program_t bf2c_parse_text(char const* text) {
    token_vec_t tokens = bf2c_parse_tokens_from_text(text);
    program_t program  = bf2c_parse_program(&tokens);
    token_vec_destroy(&tokens);
    return program;
}
