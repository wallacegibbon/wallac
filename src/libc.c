#include <stdlib.h>
#include "limits.h"
#include "os.h"

struct vfpf_buff {
	char *p;
	int idx;
};

int vfpf_flush(int fd, struct vfpf_buff *buff)
{
	os_write(fd, buff->p, buff->idx);
	buff->idx = 0;

	return 1;
}

int vfpchar(int fd, char ch, struct vfpf_buff *buff)
{
	if (buff->idx == VFPF_BUFF_SIZE)
		vfpf_flush(fd, buff);

	*(buff->p + buff->idx) = ch;

	buff->idx += 1;

	return 1;
}

int fmt_hexdigit(int num)
{
	if (num >= 10)
		return 'a' + num - 10;
	else
		return '0' + num;
}

int fmt_digit(int num, int base)
{
	if (base == 16)
		return fmt_hexdigit(num);
	else
		return '0' + num;
}

int vfpinteger_active(int fd, int num, int base, struct vfpf_buff *buff)
{
	char *p, *b;
	int i;

	b = alloca(12);
	p = b;

	for (i = 0; i == 0 || num; num /= base, i++)
		*p++ = fmt_digit(num % base, base);

	for (; i; i--)
		vfpchar(fd, *(b + i - 1), buff);

	return 1;
}

int vfpinteger_negative(int fd, int num, int base, struct vfpf_buff *buff)
{
	vfpchar(fd, '-', buff);
	vfpinteger_active(fd, -num, base, buff);

	return 1;
}

int vfpinteger(int fd, int num, int base, struct vfpf_buff *buff)
{
	if (num >= 0)
		vfpinteger_active(fd, num, base, buff);
	else
		vfpinteger_negative(fd, num, base, buff);

	return 1;
}

int vfpstring(int fd, char *s, struct vfpf_buff *buff)
{
	char ch;

	for (; ch = *s, ch; s++)
		vfpchar(fd, ch, buff);

	return 1;
}

int vfpstring_empty(int fd, struct vfpf_buff *buff)
{
	return vfpstring(fd, "<NULL>", buff);
}

int vfpf_integer(int fd, int base, char **pap, struct vfpf_buff *buff)
{
	int r;

	r = vfpinteger(fd, *((int *)*pap), base, buff);
	*pap += sizeof(int);
	return r;
}

int vfpf_string(int fd, char **pap, struct vfpf_buff *buff)
{
	char *s;
	int r;

	s = *((char **)*pap);
	if (!s)
		r = vfpstring_empty(fd, buff);
	else
		r = vfpstring(fd, s, buff);

	*pap += sizeof(char **);
	return r;
}

int vfpf_char(int fd, char **pap, struct vfpf_buff *buff)
{
	int r;

	r = vfpchar(fd, **pap, buff);
	*pap += sizeof(int);
	return r;
}

int vfpf_holder(int fd, char ch, char **pap, struct vfpf_buff *buff)
{
	if (ch == 'x')
		return vfpf_integer(fd, 16, pap, buff);
	if (ch == 'd')
		return vfpf_integer(fd, 10, pap, buff);
	if (ch == 'o')
		return vfpf_integer(fd, 8, pap, buff);
	if (ch == 's')
		return vfpf_string(fd, pap, buff);
	if (ch == 'c')
		return vfpf_char(fd, pap, buff);
	if (ch == '%')
		return vfpchar(fd, '%', buff);
	return -1;
}

int vfpf_recur(int fd, char *fmt, char *ap, struct vfpf_buff *buff)
{
	int ch, r;

	ch = *fmt;

	if (ch == '%')
		r = vfpf_holder(fd, *(++fmt), &ap, buff);
	else
		r = vfpchar(fd, ch, buff);

	if (r < 0)
		return r;

	fmt += 1;

	if (*fmt)
		return vfpf_recur(fd, fmt, ap, buff);
	else
		return 1;
}

int vfpf(int fd, char *fmt, char *ap)
{
	struct vfpf_buff buff;
	int r;

	if (!*fmt)
		return 1;

	buff.p = alloca(VFPF_BUFF_SIZE);
	buff.idx = 0;

	r = vfpf_recur(fd, fmt, ap, &buff);
	vfpf_flush(fd, &buff);

	return r;
}

int fpf(int fd, char *fmt, ...)
{
	char *ap;
	int i;

	i = sizeof(int) - 1;
	ap = (char *)&fmt + ((sizeof(fmt) + i) & ~i);

	i = vfpf(fd, fmt, ap);

	return i;
}

int vpf(char *fmt, char *ap)
{
	return vfpf(1, fmt, ap);
}

int pf(char *fmt, ...)
{
	char *ap;
	int i;

	i = sizeof(int) - 1;
	ap = (char *)&fmt + ((sizeof(fmt) + i) & ~i);

	i = vfpf(1, fmt, ap);

	return i;
}

int scmpn(char *s1, char *s2, int n)
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

int scmp(char *s1, char *s2)
{
	int t;

	for (t = 0; *s1 && *s2 && !t;)
		t = *s1++ - *s2++;

	if (!t && *s1 && !*s2)
		return 1;
	if (!t && *s2 && !*s1)
		return -1;

	return t;
}

int scpyn(char *dst, char *src, int n)
{
	int i;

	for (i = 0; i < n && *src; i++)
		*dst++ = *src++;
	*dst = '\0';

	return i;
}

int scpy(char *dst, char *src)
{
	int i;

	for (i = 0; *src; i++)
		*dst++ = *src++;
	*dst = '\0';

	return i;
}

int slen(char *s)
{
	int i;
	for (i = 0; *s && i < INT_MAX; s++)
		i++;

	if (*s)
		return -1;
	else
		return i;
}
