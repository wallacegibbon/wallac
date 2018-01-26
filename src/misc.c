#include <stdlib.h>
#include "vars.h"
#include "limits.h"
#include "misc.h"
#include "libc.h"
#include "os.h"


char *
copy_string(char *buffer)
{
  char *p;
  int i;

  i = slen(buffer);
  if (i == -1)
    exit_with("copy_string, string too long\n");

  p = malloc(i + 1);
  if (!p)
    exit_with("copy_string, failed on malloc\n");

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

  for (; *out != '.' && *out; )
    out++;

  *out++ = '.';
  *out++ = 's';
  *out = '\0';

  return 1;
}


int
purepath_of(char *orig, char *out)
{
  int i, j, l;

  j = 0;
  for (i = 0, l = slen(orig); i < l; i++)
    if (*(orig + i) == '/')
      j = i;

  if (j)
    scpyn(out, orig, j + 1);
  else
    scpy(out, "./");

  return 1;
}


char *
mkpath_from(char *origpath, char *newfile)
{
  char *buff, *p;
  int i;

  buff = alloca(slen(origpath));
  purepath_of(origpath, buff);

  i = slen(buff);
  p = malloc(i + slen(newfile) + 1);
  if (!p)
    exit_with("mkpath_from, failed on malloc\n");

  scpy(p, buff);
  scpy(p + i, newfile);

  return p;
}


void
exit_with(char *fmt, ...)
{
  char *ap;
  int i;

  i = sizeof(int) - 1;
  ap = (char *) &fmt + ((sizeof(fmt) + i) & ~i);

  vfpf(2, fmt, ap);

  os_exit(1);
}


