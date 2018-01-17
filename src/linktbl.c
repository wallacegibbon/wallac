#include <stdlib.h>
#include "misc.h"
#include "limits.h"
#include "linktbl.h"



struct linktbl *
new_linktbl()
{
  struct linktbl *p;

  p = malloc(sizeof(struct linktbl));
  if (!p)
    exit_with("Failed alloc memory for link table\n");

  p->size = 0;
  p->chain = NULL;

  return p;
}


int
linktbl_increase_size(struct linktbl *l)
{
  if (l->size == INT_MAX)
    return -1;

  l->size += 1;
  return 1;
}


int
linktbl_put(struct linktbl *l, char *key, void *value)
{
  int r;

  r = 1;
  if (l->chain)
    r = tblnode_append(l->chain, key, value);
  else
    l->chain = new_tblnode(key, value);

  if (r > 0)
    linktbl_increase_size(l);

  return r;
}


struct tblnode *
linktbl_get(struct linktbl *l, char *key)
{
  return tblnode_get(l->chain, key);
}


int
linktbl_keyexist(struct linktbl *l, char *key)
{
  return tblnode_keyexist(l->chain, key);
}


int
linktbl_print(struct linktbl *l)
{
  tblnode_print(l->chain);

  return 1;
}


int
linktbl_size(struct linktbl *l)
{
  return l->size;
}


int
linktbl_free(struct linktbl *l)
{
  tblnode_free(l->chain);
  free(l);

  return 1;
}


