#include "../../src/linklst.h"
#include "../../src/libc.h"


int
main(int argc, char **argv)
{
  struct linklst *l;
  int i;

  l = new_linklst();

  linklst_push(l, (void *) 1);
  linklst_push(l, (void *) 2);
  linklst_push(l, (void *) 3);
  linklst_push(l, (void *) 4);
  linklst_push(l, (void *) 5);

  linklst_print(l);
  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  linklst_print(l);

  linklst_push(l, (void *) 6);

  linklst_print(l);

  return 0;
}

