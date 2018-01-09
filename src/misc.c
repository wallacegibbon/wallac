#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vars.h"
#include "limits.h"
#include "misc.h"


char *
copy_of_buffer(char *buffer)
{
  char *p;

  p = malloc(strlen(buffer) + 1);
  if (!p)
    exit_with_info("Failed to malloc memory for copy_of_buffer\n");

  strcpy(p, buffer);
  return p;
}


int
is_valid_filename(char *filename)
{
  return strlen(filename) < MAX_FILENAME_SIZE - 3;
}


int
upper_case(char ch)
{
  return ch & ~(1 << 5);
}


void
init_outputname(char *outname, char *inname)
{
  strcpy(outname, inname);
  while (*outname != '.' && *outname != '\0')
    outname++;

  *outname++ = '.';
  *outname++ = 's';
  *outname = '\0';
}


void
exit_with_info(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vfprintf(stderr, fmt, ap);

  exit(0);
}


