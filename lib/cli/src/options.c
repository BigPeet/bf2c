#include "options.h"
#include <assert.h>


static void print_option(cli_option_t const* option)
{
  assert(option);
  if (option->short_name) {
    printf("-%s, ", option->short_name);
  } else {
    printf("    ");
  }
  printf("--%s", option->long_name);
  if (option->parameter) { printf(" <%s>", option->parameter); }
  printf(" %s\n", option->description);
}

void print_usage(cli_options_t const* options)
{
  assert(options);
  printf("Usage: %s [options]\n\n%s\n\nOptions:\n",
    options->program_name,
    options->program_description);
  for (size_t i = 0; i < options->count; ++i) {
    print_option(&options->options[i]);
  }
}
