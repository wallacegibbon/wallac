#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vars.h"
#include "misc.h"
#include "limit.h"



int
handle_outfile_argument(char **argv, char **end)
{
  if (argv == end)
    exit_with_info("\"-o\" should follow output filename\n");

  if (!IS_VALID_FILENAME(*argv))
    exit_with_info("output filename invalid\n");

  strcpy(filename_out, *argv);
  return walk_arguments(argv + 1, end);
}


int
handle_infile_argument(char **argv, char **end)
{
  if (!IS_VALID_FILENAME(*argv))
    exit_with_info("input filename invalid\n");

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

  if (*argv[0] != '-')
    return handle_infile_argument(argv, end);

  exit_with_info("Unknown argument: \"%s\"\n", *argv);

  return 0;
}


void
exit_with_info(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vfprintf(stderr, fmt, ap);

  exit(0);
}


void
init_outputname(char *outname, char *inname)
{
  strcpy(outname, inname);
  while (*outname++ != '.');
  *outname = 's';
}

