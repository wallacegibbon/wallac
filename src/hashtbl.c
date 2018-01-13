#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include "hashtbl.h"



struct hashtbl *
new_hashtbl(int bucketsize)
{
  struct hashtbl *p;
  int i;

  p = malloc(sizeof(struct hashtbl));
  if (!p)
    exit_with_info("Failed alloc memory for hash table\n");

  p->bucketsize = bucketsize;

  p->bucket = malloc(sizeof(struct hashnode *) * bucketsize);
  if (!p->bucket)
    exit_with_info("Failed alloc memory for hash bucket\n");

  for (i = 0; i < bucketsize; i++)
    *(p->bucket + i) = NULL;

  return p;
}


struct hashnode *
new_hashnode(char *key, void *value)
{
  struct hashnode *n;

  n = malloc(sizeof(struct hashnode));
  if (!n)
    exit_with_info("Failed alloc memory for hash node\n");

  n->key = key;
  n->value = value;
  n->next = NULL;

  return n;
}


int
strhash(char *str)
{
  unsigned int i;

  for (i = 0; *str; str++)
    i = i * 131 + *str;

  return i;
}


int
hash_keyexist(struct hashtbl *h, char *key)
{
  struct hashnode *p;
  int i;

  i = strhash(key) % h->bucketsize;
  p = *(h->bucket + i);

  if (!p)
    return 0;

  for (; p && scmp(p->key, key) != 0; p = p->next);

  if (p)
    return 1;
  else
    return 0;
}


int
append_hashnode(struct hashnode *p, char *key, void *value)
{
  struct hashnode *t;
  int r;

  if (!p)
    return 0;

  for (; p && scmp(p->key, key); p = p->next)
    t = p;

  r = 1;
  if (!p)
    t->next = new_hashnode(key, value);
  else
    r = 0;

  return r;
}


int
hash_put(struct hashtbl *h, char *key, void *value)
{
  struct hashnode **p;
  int i, r;

  i = strhash(key) % h->bucketsize;
  p = h->bucket + i;

  r = 1;
  if (*p)
    r = append_hashnode(*p, key, value);
  else
    *p = new_hashnode(key, value);

  return r;
}


struct hashnode *
hash_get(struct hashtbl *h, char *key)
{
  struct hashnode *p;
  int i;

  i = strhash(key) % h->bucketsize;
  p = *(h->bucket + i);

  if (!p)
    return NULL;

  for (; p && scmp(p->key, key); p = p->next);

  if (!p)
    return NULL;

  return p;
}


int
print_node_chain(int idx, struct hashnode *p)
{
  printf("%d)\t", idx);

  for (; p; p = p->next)
    printf("<%s|%d>--", p->key, (int) p->value);

  printf("\n");
  return 1;
}


int
hash_print(struct hashtbl *h)
{
  int i;
  for (i = 0; i < h->bucketsize; i++)
    print_node_chain(i, *(h->bucket + i));

  return 1;
}


