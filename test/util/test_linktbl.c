#include <stdio.h>
#include "../../src/linktbl.h"
#include "../../src/tblnode.h"



int
main(int argc, char **argv)
{
  struct linktbl *p;
  struct tblnode *x;
  int i;

  p = new_linktbl(15);

  linktbl_put(p, "tom", (void *) 1);
  linktbl_put(p, "harry", (void *) 2);
  linktbl_put(p, "ron", (void *) 3);
  linktbl_put(p, "wally", (void *) 4);
  linktbl_put(p, "kim", (void *) 5);
  linktbl_put(p, "rufus", (void *) 6);
  linktbl_put(p, "anna", (void *) 7);
  linktbl_put(p, "alsa", (void *) 8);
  linktbl_put(p, "hans", (void *) 9);
  i = linktbl_put(p, "sven", (void *) 0);
  if (!i)
    printf("Failed insert sven 0\n");

  i = linktbl_put(p, "olaf", (void *) 1);
  if (!i)
    printf("Failed insert olaf 1\n");
  i = linktbl_put(p, "olaf", (void *) 2);
  if (!i)
    printf("Failed insert olaf 2\n");

  linktbl_print(p);

  printf("\n\n");

  x = linktbl_get(p, "olaf");
  if (!x)
    printf("failed get from linktable\n");
  else
    printf("%s: %d\n", x->key, (int) x->value);

  return 0;
}
