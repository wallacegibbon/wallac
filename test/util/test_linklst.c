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

  linklst_print(l);

  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  linklst_print(l);

  linklst_push(l, (void *) 4);
  linklst_push(l, (void *) 5);

  linklst_print(l);

  i = (int) linklst_shift(l);
  pf("i is %d\n", i);

  i = (int) linklst_shift(l);
  pf("i is %d\n", i);

  i = (int) linklst_shift(l);
  pf("i is %d\n", i);

  linklst_print(l);

  linklst_push(l, (void *) 6);
  linklst_push(l, (void *) 7);
  linklst_push(l, (void *) 8);

  i = (int) linklst_shift(l);
  pf("i is %d\n", i);

  i = (int) linklst_pop(l);
  pf("i is %d\n", i);

  return 0;
}

