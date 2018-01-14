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

  if (!t && *s1 && !*s2 && i < n)
    return 1;
  if (!t && *s2 && !*s1 && i < n)
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
    exit_with("copy_of_buffer, string too long\n");

  p = malloc(i + 1);
  if (!p)
    exit_with("copy_of_buffer, failed on malloc\n");

  scpy(p, buffer);
  return p;
}


int
is_valid_filename(char *filename)
{
  int i;

  i = slen(filename);
  if (i == -1)
    exit_with("is_valid_filename, string(filename) too long\n");

  return i < MAX_FILENAME_SIZE - 3;
}


int
upper_case(char ch)
{
  return ch & ~(1 << 5);
}


int
init_pathname_out(char *pathname, char *out)
{
  int i, j, l;

  j = 0;
  for (i = 0, l = slen(pathname); i < l; i++)
    if (*(pathname + i) == '/')
      j = i;

  if (j)
    scpy(out, pathname + j + 1);
  else
    scpy(out, pathname);

  for (; *out != '.' && *out; out++);

  *out++ = '.';
  *out++ = 's';
  *out = '\0';

  return 1;
}

int
init_srcpath(char *pathname, char *srcpath)
{
  int i, j, l;

  j = 0;
  for (i = 0, l = slen(pathname); i < l; i++)
    if (*(pathname + i) == '/')
      j = i;

  if (j)
    scpyn(srcpath, pathname, j + 1);

  return 1;
}


void
exit_with(char *fmt, ...)
{
  char *ap;
  int i;

  i = sizeof(int) - 1;
  ap = (char *) &fmt + ((sizeof(fmt) + i) & ~i);

  vprintf(fmt, ap);

  exit(0);
}


