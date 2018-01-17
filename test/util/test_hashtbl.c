#include <stdio.h>
#include "../../src/hashtbl.h"
#include "../../src/tblnode.h"



int
main(int argc, char **argv)
{
  struct hashtbl *p;
  struct tblnode *x;
  int i;

  p = new_hashtbl(15);

  hashtbl_put(p, "tom", (void *) 1);
  hashtbl_put(p, "harry", (void *) 2);
  hashtbl_put(p, "ron", (void *) 3);
  hashtbl_put(p, "wally", (void *) 4);
  hashtbl_put(p, "kim", (void *) 5);
  hashtbl_put(p, "rufus", (void *) 6);
  hashtbl_put(p, "anna", (void *) 7);
  hashtbl_put(p, "alsa", (void *) 8);
  hashtbl_put(p, "hans", (void *) 9);
  i = hashtbl_put(p, "sven", (void *) 0);
  if (!i)
    printf("Failed insert sven 0\n");

  i = hashtbl_put(p, "olaf", (void *) 1);
  if (!i)
    printf("Failed insert olaf 1\n");
  i = hashtbl_put(p, "olaf", (void *) 2);
  if (!i)
    printf("Failed insert olaf 2\n");

  hashtbl_print(p);

  printf("\n\n");

  x = hashtbl_get(p, "olaf");
  if (!x)
    printf("failed get from hashtable\n");
  else
    printf("%s: %d\n", x->key, (int) x->value);

  return 0;
}
