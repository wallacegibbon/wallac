#include <stdlib.h>
#include "limits.h"
#include "os.h"


struct vfpf_buff { char *p; int idx; };


int
vfpf_flush(int fd, struct vfpf_buff *buff)
{
  os_write(fd, buff->p, buff->idx);
  buff->idx = 0;

  return 1;
}


int
vfpchar(int fd, char ch, struct vfpf_buff *buff)
{
  if (buff->idx == VFPF_BUFF_SIZE)
    vfpf_flush(fd, buff);

  *(buff->p + buff->idx) = ch;

  buff->idx += 1;

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


int
vfpinteger(int fd, int num, int base, struct vfpf_buff *buff)
{
  char *p, *b;
  int i;

  if (num == 0)
    goto p_0;

  b = alloca(12);

  for (p = b, i = 0; num; num /= base, i++)
    *p++ = fmt_digit(num % base, base);

  for (; i; i--)
    vfpchar(fd, *(b + i - 1), buff);

  return 1;

p_0:
  vfpchar(fd, '0', buff);
  return 1;
}


int
vfpstring(int fd, char *s, struct vfpf_buff *buff)
{
  char ch;

  for (; ch = *s, ch; s++)
    vfpchar(fd, ch, buff);

  return 1;
}


int
vfpf(int fd, char *fmt, char *ap)
{
  struct vfpf_buff buff;
  int ch;

  buff.p = alloca(VFPF_BUFF_SIZE);
  buff.idx = 0;

start:
  ch = *fmt;

  if (!ch)
    goto finish;
  if (ch == '%')
    goto dispatch;

  vfpchar(fd, ch, &buff);
  fmt++;
  goto start;

dispatch:
  ch = *(++fmt);
  fmt++;

  if (ch == 'x')
    goto f_x;
  if (ch == 'd')
    goto f_d;
  if (ch == 'o')
    goto f_o;
  if (ch == 's')
    goto f_s;
  if (ch == 'c')
    goto f_c;

  goto error;

f_x:
  vfpinteger(fd, *((int *) ap), 16, &buff);
  ap += sizeof(int);
  goto start;

f_d:
  vfpinteger(fd, *((int *) ap), 10, &buff);
  ap += sizeof(int);
  goto start;

f_o:
  vfpinteger(fd, *((int *) ap), 8, &buff);
  ap += sizeof(int);
  goto start;

f_s:
  vfpstring(fd, *((char **) ap), &buff);
  ap += sizeof(char **);
  goto start;

f_c:
  vfpchar(fd, *ap, &buff);
  ap += sizeof(int);
  goto start;

finish:
  vfpf_flush(fd, &buff);
  return 1;

error:
  vfpf_flush(fd, &buff);
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


