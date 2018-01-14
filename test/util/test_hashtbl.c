#include <stdio.h>
#include "../../src/hashtbl.h"



int
main(int argc, char **argv)
{
  struct hashtbl *p;
  struct hashnode *x;
  int i;

  p = new_hashtbl(15);

  hash_put(p, "tom", (void *) 1);
  hash_put(p, "harry", (void *) 2);
  hash_put(p, "ron", (void *) 3);
  hash_put(p, "wally", (void *) 4);
  hash_put(p, "kim", (void *) 5);
  hash_put(p, "rufus", (void *) 6);
  hash_put(p, "anna", (void *) 7);
  hash_put(p, "alsa", (void *) 8);
  hash_put(p, "hans", (void *) 9);
  i = hash_put(p, "sven", (void *) 0);
  if (!i)
    printf("Failed insert sven 0\n");

  i = hash_put(p, "olaf", (void *) 1);
  if (!i)
    printf("Failed insert olaf 1\n");
  i = hash_put(p, "olaf", (void *) 2);
  if (!i)
    printf("Failed insert olaf 2\n");

  hash_print(p);

  printf("\n\n");

  x = hash_get(p, "olaf");
  if (!x)
    printf("failed get from hashtable\n");
  else
    printf("%s: %d\n", x->key, (int) x->value);

  return 0;
}
