#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


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


