#include "limits.h"
#include "misc.h"
#include "vars.h"
#include "cmdargs.h"


int
handle_outfile_argument(char **argv, char **end)
{
  if (argv == end)
    exit_with_info("\"-o\" should follow output filename\n");

  if (!is_valid_filename(*argv))
    exit_with_info("output filename invalid:\"%s\"\n", *argv);

  scpy(filename_out, *argv);
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
  if (!is_valid_filename(*argv))
    exit_with_info("input filename invalid: \"%s\"\n", *argv);

  scpy(filename, *argv);
  return walk_arguments(argv + 1, end);
}


int
walk_arguments(char **argv, char **end)
{
  if (argv == end)
    return 0;

  if (scmp(*argv, "-o") == 0)
    return handle_outfile_argument(argv + 1, end);

  if (scmp(*argv, "--verbose") == 0)
    return handle_verbose(argv + 1, end);

  if (**argv != '-')
    return handle_infile_argument(argv, end);

  exit_with_info("Unknown argument: \"%s\"\n", *argv);

  return 0;
}


