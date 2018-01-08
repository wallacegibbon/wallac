#include <string.h>
#include "vars.h"
#include "misc.h"
#include "limits.h"
#include "cmdargs.h"



int
handle_outfile_argument(char **argv, char **end)
{
  if (argv == end)
    exit_with_info("\"-o\" should follow output filename\n");

  if (!IS_VALID_FILENAME(*argv))
    exit_with_info("output filename invalid:\"%s\"\n", *argv);

  strcpy(filename_out, *argv);
  return walk_arguments(argv + 1, end);
}


int
handle_verbose(char **argv, char **end)
{
  verbose = 1;
  return walk_arguments(argv, end);
}


int
handle_infile_argument(char **argv, char **end)
{
  if (!IS_VALID_FILENAME(*argv))
    exit_with_info("input filename invalid: \"%s\"\n", *argv);

  strcpy(filename, *argv);
  return walk_arguments(argv + 1, end);
}


int
walk_arguments(char **argv, char **end)
{
  if (argv == end)
    return 0;

  if (strcmp(*argv, "-o") == 0)
    return handle_outfile_argument(argv + 1, end);

  if (strcmp(*argv, "--verbose") == 0)
    return handle_verbose(argv + 1, end);

  if (*argv[0] != '-')
    return handle_infile_argument(argv, end);

  exit_with_info("Unknown argument: \"%s\"\n", *argv);

  return 0;
}


