#include <stdlib.h>
#include "linklst.h"
#include "limits.h"
#include "misc.h"
#include "libc.h"


struct lstnode *
new_lstnode(void *value)
{
  struct lstnode *n;

  n = malloc(sizeof(struct lstnode));
  if (!n)
    exit_with("Failed alloc memory for list node\n");

  n->value = value;
  n->next = NULL;
  n->prev = NULL;

  return n;
}


struct linklst *
new_linklst()
{
  struct linklst *p;

  p = malloc(sizeof(struct linklst));
  if (!p)
    exit_with("Failed alloc memory for link list\n");

  p->size = 0;
  p->chain = NULL;
  p->cursor = NULL;

  return p;
}


int
lstnode_free(struct lstnode *n)
{
  free(n);
  return 1;
}


int
linklst_increase_size(struct linklst *l)
{
  if (l->size == INT_MAX)
    exit_with("linklst size too big\n");

  l->size += 1;
  return 1;
}


int
linklst_decrease_size(struct linklst *l)
{
  if (l->size > 0)
    l->size -= 1;

  return 1;
}


int
linklst_push_1st(struct linklst *l, struct lstnode *n)
{
  linklst_increase_size(l);

  l->chain = n;
  l->cursor = n;

  return 1;
}


int
linklst_push_normal(struct linklst *l, struct lstnode *n)
{
  linklst_increase_size(l);

  n->prev = l->cursor;
  l->cursor->next = n;
  l->cursor = n;

  return 1;
}


int
linklst_push(struct linklst *l, void *value)
{
  struct lstnode *n;

  n = new_lstnode(value);

  if (l->chain)
    linklst_push_normal(l, n);
  else
    linklst_push_1st(l, n);

  return 1;
}


void *
linklst_pop(struct linklst *l)
{
  struct lstnode *n;
  void *r;

  n = l->cursor;
  if (!n)
    return NULL;

  l->cursor = l->cursor->prev;
  if (l->cursor)
    l->cursor->next = NULL;
  else
    l->chain = NULL;

  linklst_decrease_size(l);

  r = n->value;
  lstnode_free(n);

  return r;
}


void *
linklst_shift(struct linklst *l)
{
  struct lstnode *n;
  void *r;

  n = l->chain;
  if (!n)
    return NULL;

  l->chain = l->chain->next;
  if (l->chain)
    l->chain->prev = NULL;
  else
    l->cursor = NULL;

  linklst_decrease_size(l);

  r = n->value;
  lstnode_free(n);

  return r;
}


int
linklst_print(struct linklst *l)
{
  struct lstnode *n;

  for (n = l->chain; n; n = n->next)
    pf("<%d>--", (int) n->value);

  pf("\n");

  return 1;
}


