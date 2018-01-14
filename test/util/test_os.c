#include <stdlib.h>
#include <stdio.h>
#include "../../src/os.h"


int
test_basic_op()
{
  char *buff;
  int fd, i;

  buff = malloc(100);
  if (!buff)
    printf("Failed alloc memory for buff\n");

  printf("Trying to test os related functions...\n");

  fd = os_open_wr("/tmp/a.txt");
  printf("fd: %d\n", fd);

  i = os_write(fd, "hello, world !\n", 15);
  printf("write result: %d\n", i);

  i = os_close(fd);
  printf("close result: %d\n", i);

  fd = os_open_rd("/tmp/a.txt");
  i = os_read(fd, buff, 10);
  printf("read result: %d\n", i);
  printf("read content: %s\n", buff);

  os_close(fd);
  printf("close result: %d\n", i);

  fd = os_open_rdwr("/tmp/a.txt");
  i = os_read(fd, buff, 10);
  printf("read result: %d\n", i);
  printf("read content: %s\n", buff);
  i = os_write(fd, "*******", 7);
  printf("write result: %d\n", i);

  os_close(fd);
  printf("close result: %d\n", i);

  os_exit(2);

  return 0;
}

int
main(int argc, char **argv)
{
  int fd;

  //test_basic_op();
  fd = os_open_rd("/tmp/a.txt");
  printf("fd: %d\n", fd);

  fd = os_open_rd("/tmp/a.txt");
  printf("fd: %d\n", fd);

  fd = os_open_rd("/tmp/a.txt");
  printf("fd: %d\n", fd);

  fd = os_open_rd("/tmp/a.txt");
  printf("fd: %d\n", fd);

  return 0;
}
