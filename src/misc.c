#include <string.h>
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


