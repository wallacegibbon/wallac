#include <stdio.h>
#include "../../src/hashtbl.h"



int
main(int argc, char **argv)
{
  struct hashtbl *p;
  int x;
  int i;

  p = new_hashtbl(15);

  hashtbl_add(p, "tom", (void *) 1);
  hashtbl_add(p, "harry", (void *) 2);
  hashtbl_add(p, "ron", (void *) 3);
  hashtbl_add(p, "wally", (void *) 4);
  hashtbl_add(p, "kim", (void *) 5);
  hashtbl_add(p, "rufus", (void *) 6);
  hashtbl_add(p, "anna", (void *) 7);
  hashtbl_add(p, "alsa", (void *) 8);
  hashtbl_add(p, "hans", (void *) 9);
  i = hashtbl_add(p, "sven", (void *) 10);
  if (!i)
    printf("Failed insert sven 10\n");

  i = hashtbl_add(p, "olaf", (void *) 11);
  if (!i)
    printf("Failed insert olaf 11\n");
  i = hashtbl_add(p, "olaf", (void *) 12);
  if (!i)
    printf("Failed insert olaf 12\n");

  hashtbl_print(p);

  printf("\n\n");

  x = (int) hashtbl_get(p, "olaf");
  printf("%s: %d\n", "olaf", x);

  return 0;
}
