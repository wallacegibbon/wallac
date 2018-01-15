#include <stdlib.h>
#include "limits.h"
#include "os.h"


// For now, vfpf is not thread safe
char *fpchar_buff;
int fpchar_idx;



int
init_libc()
{
  fpchar_buff = malloc(FPCHAR_BUFF_SIZE);
  if (!fpchar_buff)
    return -1;

  fpchar_idx = 0;

  return 1;
}


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



int
flush_fpchar_buff(int fd)
{
  os_write(fd, fpchar_buff, fpchar_idx);
  fpchar_idx = 0;

  return 1;
}


int
vfpchar(int fd, char ch)
{
  if (fpchar_idx == FPCHAR_BUFF_SIZE)
    flush_fpchar_buff(fd);

  *(fpchar_buff + fpchar_idx) = ch;

  fpchar_idx++;

  return 1;
}


int
fmt_hexdigit(int num)
{
  if (num >= 10)
    return 'a' + num - 10;
  else
    return '0' + num;
}


int
fmt_digit(int num, int base)
{
  if (base == 16)
    return fmt_hexdigit(num);
  else
    return '0' + num;
}


char *
vfpf_p_int(int fd, char *ap, int base)
{
  char *p, *buff;
  int i, m, n;

  n = *((int *) ap);
  if (n == 0)
    goto p_0;

  buff = alloca(12);

  for (p = buff, i = 0; n; n /= base, i++)
    *p++ = fmt_digit(n % base, base);

  for (; i; i--)
    vfpchar(fd, *(buff + i - 1));

  ap += sizeof(int);
  return ap;

p_0:
  vfpchar(fd, '0');

  ap += sizeof(int);
  return ap;
}


char *
vfpf_p_str(int fd, char *ap)
{
  char *s, ch;

  s = *((char **) ap);

  for (; ch = *s, ch; s++)
    vfpchar(fd, ch);

  ap += sizeof(char **);
  return ap;
}


char *
vfpf_p_ch(int fd, char *ap)
{
  vfpchar(fd, *ap);
  ap += sizeof(int);

  return ap;
}


char *
vfpf_holder(int fd, int type, char *ap)
{
  if (type == 'x')
    return vfpf_p_int(fd, ap, 16);

  if (type == 'd')
    return vfpf_p_int(fd, ap, 10);

  if (type == 'o')
    return vfpf_p_int(fd, ap, 8);

  if (type == 's')
    return vfpf_p_str(fd, ap);

  if (type == 'c')
    return vfpf_p_ch(fd, ap);

  return NULL;
}


int
vfpf(int fd, char *fmt, char *ap)
{
  int ch;

start:
  ch = *fmt;

  if (!ch)
    goto finish;

  if (ch == '%')
    ap = vfpf_holder(fd, *(++fmt), ap);
  else
    vfpchar(fd, ch);

  if (ap == NULL)
    goto error;

  fmt++;
  goto start;

finish:
  flush_fpchar_buff(fd);
  return 1;

error:
  flush_fpchar_buff(fd);
  return -1;
}


int
fpf(int fd, char *fmt, ...)
{
  char *ap;
  int i;

  i = sizeof(int) - 1;
  ap = (char *) &fmt + ((sizeof(fmt) + i) & ~i);

  i = vfpf(fd, fmt, ap);

  return i;
}


int
vpf(char *fmt, char *ap)
{
  return vfpf(1, fmt, ap);
}


int
pf(char *fmt, ...)
{
  char *ap;
  int i;

  i = sizeof(int) - 1;
  ap = (char *) &fmt + ((sizeof(fmt) + i) & ~i);

  i = vfpf(1, fmt, ap);

  return i;
}


