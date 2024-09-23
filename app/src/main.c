#include "options.h"

INIT_OPTIONS(cli_options,
  "bf2c",
  "A Brainf*ck to C transpiler",
  OPTION("input",
    "i",
    "FILE",
    "\tInput file. Uses stdin, if not provided.",
    STRING,
    NULL),
  OPTION("output",
    "o",
    "FILE",
    "\tOutput file. Uses stdout, if not provided.",
    STRING,
    NULL),
  FLAG("verbose", "v", "\t\tVerbose output"),
  FLAG("dynamic-memory", "d", "\tUse dynamic memory allocation"),
  FLAG("help", "h", "\t\tPrint this help message"),
  FLAG("version", NULL, "\t\tPrint version"), );

int main(void)
{
  // TODO: implement parse_args function
  print_usage(&cli_options);
  return 0;
}
