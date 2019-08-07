#ifndef __OS_H__
#define __OS_H__

int os_open_rdwr(char *pathname);

int os_open_wr(char *pathname);

int os_open_rd(char *pathname);

int os_close(int fd);

int os_read(int fd, void *buf, int count);

int os_write(int fd, void *buf, int count);

void os_exit(int status);

#endif
