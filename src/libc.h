#ifndef __LIBC_H__
#define __LIBC_H__

int scmpn(char *s1, char *s2, int n);

int scmp(char *s1, char *s2);

int scpyn(char *dst, char *src, int n);

int scpy(char *dst, char *src);

int slen(char *s);

int vfpf(int fd, char *fmt, char *ap);

int fpf(int fd, char *fmt, ...);

int vpf(char *fmt, char *ap);

int pf(char *fmt, ...);

#endif
