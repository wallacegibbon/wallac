#include <stdlib.h>
#include <stdio.h>

#include "compile.h"
#include "limits.h"
#include "misc.h"
#include "cmdargs.h"


char *filename_src, *filename_out;
int verbose;


int
main(int argc, char **argv)
{
  char *p;

  if (argc == 1)
    exit_with("Usage: wcc myfile.c [-o outputfile]\n");

  verbose = 0;

  filename_src = malloc(MAX_FILENAME_SIZE);
  if (!filename_src)
    exit_with("Failed alloc memory for filename_src\n");

  filename_out = malloc(MAX_FILENAME_SIZE);
  if (!filename_out)
    exit_with("Failed alloc memory for filename_out\n");

  walk_arguments(argv + 1, argv + argc);

  if (*filename_src == '\0')
    exit_with("filename_src is not given\n");

  if (*filename_out == '\0')
    init_outputname(filename_out, filename_src);

  printf("Will compiling %s to %s\n", filename_src, filename_out);

  if (!scmp(filename_src, filename_out))
    exit_with("input and output file can't be the same\n");

  compile();

  return 0;
}


