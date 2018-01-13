#include <stdio.h>

#include "compile.h"
#include "limits.h"
#include "misc.h"
#include "cmdargs.h"


char filename_src[MAX_FILENAME_SIZE], filename_out[MAX_FILENAME_SIZE];

int verbose;


int
main(int argc, char **argv)
{
  char *p;

  if (argc == 1)
    exit_with_info("Usage: cc myfile.c [-c] [-o outputfile]\n");

  verbose = 0;

  walk_arguments(argv + 1, argv + argc);

  if (*filename_src == '\0')
    exit_with_info("filename_src is not given\n");

  if (*filename_out == '\0')
    init_outputname(filename_out, filename_src);

  printf("Will compiling %s to %s\n", filename_src, filename_out);

  if (!scmp(filename_src, filename_out))
    exit_with_info("input and output file can't be the same\n");

  compile();

  return 0;
}


