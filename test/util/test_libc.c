#include "../../src/libc.h"

int
main(int argc, char **argv)
{
  init_libc();

  pf("hello, world !, %s, %x, %d, %o.\n", "wallace", 0x1234, 1234, 01234);
  return 0;
}
