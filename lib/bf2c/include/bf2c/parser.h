#ifndef BF2C_PARSER_H_
#define BF2C_PARSER_H_

#include <stdio.h>
#include "bf2c/program.h"

//program_t* bf2c_parse_file(FILE* file); OR a RESULT
program_t bf2c_parse_file(FILE* file);
program_t bf2c_parse_filename(char const* filename);

#endif /* ifndef BF2C_PARSER_H_ */
