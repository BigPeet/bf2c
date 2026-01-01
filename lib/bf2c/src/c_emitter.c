#include "bf2c/c_emitter.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf2c/command.h"
#include "bf2c/program.h"
#include "core/vector.h"

enum {
    INDENT_WIDTH = 4,
    // used to store the command string
    // before writing it to the file
    // (e.g. "data[idx] += 1;")
    // 256 should be enough for most cases
    BUFFER_SIZE = 256
};

#define DBG_SIZE_VAL  "31"
#define DATA_SIZE_VAL "30000"

static char const* const INCLUDES = "#include <stdio.h>\n\n";
static char const* const PREAMBLE = "/* PREAMBLE */\n"
                                    "#define DATA_SIZE " DATA_SIZE_VAL "\n";
static char const* const DEBUG_FUNC =
    "#define DBG_SIZE " DBG_SIZE_VAL "\n\n"
    "void debug(unsigned char const* data, unsigned int idx) {\n"
    "    size_t const start = idx < DBG_SIZE / 2 ? 0\n"
    "                       : idx >= DATA_SIZE - DBG_SIZE / 2 ? DATA_SIZE - DBG_SIZE\n"
    "                       : idx - DBG_SIZE / 2;\n"
    "    printf(\"\\n\");\n"
    "    for (size_t i = start; i < DBG_SIZE; ++i) {\n"
    "        printf(\"[%3d]\", data[i]);\n"
    "    }\n"
    "    printf(\"\\n\");\n"
    "}\n";
static char const* const MAIN_SETUP = "\nint main(void) {\n"
                                      "    unsigned char data[DATA_SIZE] = {0};\n"
                                      "    unsigned int idx = 0;\n"
                                      "    /* PROGRAM */\n";
static char const* const EPILOGUE   = "    /* PROGRAM END */\n"
                                      "    return 0;\n"
                                      "}\n";

static bool bf2c_emit_preamble(FILE* file, program_t const* program) {
    if (command_vec_contains(&program->commands, (command_t){.type = COMMAND_TYPE_DEBUG})) {
        return fprintf(file, "%s%s%s%s", INCLUDES, PREAMBLE, DEBUG_FUNC, MAIN_SETUP) >= 0;
    }
    if (command_vec_contains(&program->commands, (command_t){.type = COMMAND_TYPE_OUT}) ||
        command_vec_contains(&program->commands, (command_t){.type = COMMAND_TYPE_IN}))
    {
        return fprintf(file, "%s%s%s", INCLUDES, PREAMBLE, MAIN_SETUP) >= 0;
    }
    return fprintf(file, "%s%s", PREAMBLE, MAIN_SETUP) >= 0;
}

static bool bf2c_emit_epilogue(FILE* file) {
    return fprintf(file, "%s", EPILOGUE) >= 0;
}

static bool bf2c_emit_command(FILE* file, command_t command, int* indentation_level) {
    assert(indentation_level);
    // TODO: improve buffer size handling
    char buffer[BUFFER_SIZE];
    int ret = 0;
    switch (command.type) {
        // TODO: improve change value and ptr handling
        case COMMAND_TYPE_CHANGE_VAL:
            ret = snprintf(buffer,
                           BUFFER_SIZE * sizeof(buffer[0]),
                           "data[idx] %c= %d;",
                           command.value > 0 ? '+' : '-',
                           abs(command.value));
            if (ret < 0 || ret >= BUFFER_SIZE) {
                return false;
            }
            break;
        case COMMAND_TYPE_CHANGE_PTR:
            ret = snprintf(buffer,
                           BUFFER_SIZE * sizeof(buffer[0]),
                           "idx %c= %d;",
                           command.value > 0 ? '+' : '-',
                           abs(command.value));
            if (ret < 0 || ret >= BUFFER_SIZE) {
                return false;
            }
            break;
        // strcpy is fine here, because all the string literals are constant
        // and known to be shorter than the buffer size.
        case COMMAND_TYPE_OUT: strcpy(buffer, "printf(\"%c\", data[idx]);"); break;
        case COMMAND_TYPE_IN:  strcpy(buffer, "(void) scanf(\"%c\", &data[idx]);"); break;
        case COMMAND_TYPE_LOOP_START:
            strcpy(buffer, "while (data[idx]) {");
            ret = 1;
            break;
        case COMMAND_TYPE_LOOP_END:
            strcpy(buffer, "}");
            --(*indentation_level);
            break;
        case COMMAND_TYPE_DEBUG:   strcpy(buffer, "debug(data, idx);"); break;
        case COMMAND_TYPE_UNKNOWN: strcpy(buffer, "");
    }
    int const res = fprintf(file, "%*c%s\n", INDENT_WIDTH * *indentation_level, ' ', buffer) >= 0;
    if (ret == 1) {
        ++(*indentation_level);
    }
    return res;
}

// TODO: return a RESULT for more precise error handling instead of bool
bool bf2c_emit_c_to_file(FILE* file, program_t const* program) {
    if (!bf2c_emit_preamble(file, program)) {
        return false;
    }
    int indentation_level = 1;
    VEC_FOR_EACH (command_t, cmd, program->commands) {
        if (!bf2c_emit_command(file, cmd, &indentation_level)) {
            return false;
        }
    }
    return bf2c_emit_epilogue(file);
}

bool bf2c_emit_c_to_filename(char const* filename, program_t const* program) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return false;
    }
    bool result = bf2c_emit_c_to_file(file, program);
    (void) fclose(file);
    return result;
}
