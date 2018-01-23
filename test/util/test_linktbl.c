#include <stdio.h>
#include "../../src/linktbl.h"



int
main(int argc, char **argv)
{
  struct linktbl *p;
  int x;
  int i;

  p = new_linktbl(15);

  linktbl_add(p, "tom", (void *) 1);
  linktbl_add(p, "harry", (void *) 2);
  linktbl_add(p, "ron", (void *) 3);
  linktbl_add(p, "wally", (void *) 4);
  linktbl_add(p, "kim", (void *) 5);
  linktbl_add(p, "rufus", (void *) 6);
  linktbl_add(p, "anna", (void *) 7);
  linktbl_add(p, "alsa", (void *) 8);
  linktbl_add(p, "hans", (void *) 9);
  i = linktbl_add(p, "sven", (void *) 10);
  if (!i)
    printf("Failed insert sven 10\n");

  i = linktbl_add(p, "olaf", (void *) 11);
  if (!i)
    printf("Failed insert olaf 11\n");
  i = linktbl_add(p, "olaf", (void *) 12);
  if (!i)
    printf("Failed insert olaf 12\n");

  linktbl_print(p);

  printf("\n\n");

  x = (int) linktbl_get(p, "olaf");
  printf("%s: %d\n", "olaf", x);

  return 0;
}
