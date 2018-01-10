#include <stdlib.h>
#include <stdio.h>
#include "vars.h"
#include "limits.h"
#include "misc.h"


int
scmpn(char *s1, char *s2, int n)
{
  int i, t;

  for (i = 0, t = 0; *s1 && *s2 && !t && i < n; i++)
    t = *s1++ - *s2++;

  if (!t && *s1 && !*s2)
    return 1;
  if (!t && *s2 && !*s1)
    return -1;

  return t;
}


int
scmp(char *s1, char *s2)
{
  int t;

  for (t = 0; *s1 && *s2 && !t; )
    t = *s1++ - *s2++;

  if (!t && *s1 && !*s2)
    return 1;
  if (!t && *s2 && !*s1)
    return -1;

  return t;
}


int
scpyn(char *dst, char *src, int n)
{
  int i;

  for (i = 0; i < n && *src; i++)
    *dst++ = *src++;
  *dst = '\0';

  return i;
}


int
scpy(char *dst, char *src)
{
  int i;

  for (i = 0; *src; i++)
    *dst++ = *src++;
  *dst = '\0';

  return i;
}


int
slen(char *s)
{
  int i;
  for (i = 0; *s && i < INT_MAX; s++)
    i++;

  if (*s)
    return -1;
  else
    return i;
}


char *
copy_of_buffer(char *buffer)
{
  char *p;
  int i;

  i = slen(buffer);
  if (i == -1)
    exit_with_info("copy_of_buffer, string too long\n");

  p = malloc(i + 1);
  if (!p)
    exit_with_info("copy_of_buffer, failed on malloc\n");

  scpy(p, buffer);
  return p;
}


int
is_valid_filename(char *filename)
{
  int i;

  i = slen(filename);
  if (i == -1)
    exit_with_info("is_valid_filename, filename too long\n");

  return i < MAX_FILENAME_SIZE - 3;
}


int
upper_case(char ch)
{
  return ch & ~(1 << 5);
}


void
init_outputname(char *outname, char *inname)
{
  scpy(outname, inname);

  for (; *outname != '.' && *outname != '\0'; )
    outname++;

  *outname++ = '.';
  *outname++ = 's';
  *outname = '\0';
}


void
exit_with_info(char *fmt, ...)
{
  char *ap;

  ap = (char *) &fmt + sizeof(int);

  vprintf(fmt, ap);

  exit(0);
}


