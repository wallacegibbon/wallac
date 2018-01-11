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


int
strhash(char *str, int bucketsize)
{
  unsigned int i;

  for (i = 0; *str; str++)
    i = i * 131 + *str;

  return i % bucketsize;
}


int
hash_keyexist(struct hashtbl *h, char *key)
{
  struct hashnode **bk, *p;
  int i;

  bk = h->bucket;

  i = strhash(key, h->bucketsize);
  bk += i;

  p = *bk;

  if (!p)
    return 0;

  for (; p && scmp(p->key, key) != 0; p = p->next);

  if (p)
    return 1;
  else
    return 0;
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
append_hashnode(struct hashnode *start, struct hashnode *n)
{
  if (!start)
    return -1;

  for (; start->next; start = start->next);

  start->next = n;
  return 1;
}


int
hash_insert(struct hashtbl *h, char *key, void *value)
{
  struct hashnode **p, *n;
  int i;

  if (hash_keyexist(h, key))
    return -1;

  n = new_hashnode(key, value);

  i = strhash(key, h->bucketsize);
  p = h->bucket + i;

  if (*p)
    append_hashnode(*p, n);
  else
    *p = n;

  return 1;
}


int
print_node_chain(int idx, struct hashnode *chain)
{
  printf("%d)\t", idx);

  for (; chain; chain = chain->next)
    printf("[%s|%d] -> ", chain->key, chain->value);

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


