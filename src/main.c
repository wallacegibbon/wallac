#include <stdlib.h>
#include <stdio.h>

#include "compile.h"
#include "limits.h"
#include "misc.h"
#include "cmdargs.h"
#include "libc.h"



char *pathname_src, *pathname_out, *path_src;
int verbose;
int debug;


int
main(int argc, char **argv)
{
  char *p;

  if (argc == 1)
    exit_with("Usage: wcc myfile.c [-o outputfile]\n");

  verbose = 0;
  debug = 0;

  pathname_src = malloc(MAX_FILENAME_SIZE);
  if (!pathname_src)
    exit_with("Failed alloc memory for pathname_src\n");

  pathname_out = malloc(MAX_FILENAME_SIZE);
  if (!pathname_out)
    exit_with("Failed alloc memory for pathname_out\n");

  path_src = malloc(MAX_FILENAME_SIZE);
  if (!path_src)
    exit_with("Failed alloc memory for path_src\n");

  *pathname_src = '\0';
  *pathname_out = '\0';
  *path_src = '\0';

  walk_arguments(argv + 1, argv + argc);

  if (*pathname_src == '\0')
    exit_with("pathname_src is not given\n");

  if (*pathname_out == '\0')
    init_pathname_out(pathname_src, pathname_out);

  init_srcpath(pathname_src, path_src);

  printf("Will compiling %s to %s\n", pathname_src, pathname_out);

  if (!scmp(pathname_src, pathname_out))
    exit_with("input and output file can't be the same\n");

  compile();

  return 0;
}


