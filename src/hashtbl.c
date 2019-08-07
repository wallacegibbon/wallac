#include <stdlib.h>
#include "misc.h"
#include "libc.h"
#include "limits.h"
#include "hashtbl.h"

struct hashtbl *new_hashtbl(int bucketsize)
{
	struct hashtbl *p;
	int i;

	p = malloc(sizeof(struct hashtbl));
	if (!p)
		exit_with("Failed alloc memory for hash table\n");

	p->bucketsize = bucketsize;
	p->size = 0;

	p->bucket = malloc(sizeof(struct tblnode *) * bucketsize);
	if (!p->bucket)
		exit_with("Failed alloc memory for hash bucket\n");

	for (i = 0; i < bucketsize; i++)
		*(p->bucket + i) = NULL;

	return p;
}

unsigned int strhash(char *str)
{
	unsigned int i;

	for (i = 0; *str; str++)
		i = i * 131 + *str;

	return i;
}

int hashtbl_increase_size(struct hashtbl *h)
{
	if (h->size == INT_MAX)
		exit_with("hashtbl size too big\n");

	h->size += 1;
	return 1;
}

struct tblnode **hashtbl_getslot(struct hashtbl *h, char *key)
{
	unsigned int i;

	i = strhash(key) % h->bucketsize;
	return h->bucket + i;
}

int hashtbl_add(struct hashtbl *h, char *key, void *value)
{
	struct tblnode **p;
	int r;

	p = hashtbl_getslot(h, key);

	r = 1;
	if (*p)
		r = tblnode_add(*p, key, value);
	else
		*p = new_tblnode(key, value);

	if (r > 0)
		hashtbl_increase_size(h);

	return r;
}

int hashtbl_set(struct hashtbl *h, char *key, void *value)
{
	struct tblnode **p;
	int r;

	p = hashtbl_getslot(h, key);

	if (!*p)
		return 0;

	r = tblnode_set(*p, key, value);

	return r;
}

void *hashtbl_get(struct hashtbl *h, char *key)
{
	struct tblnode *p;

	p = *hashtbl_getslot(h, key);
	return tblnode_get(p, key);
}

int hashtbl_keyexist(struct hashtbl *h, char *key)
{
	struct tblnode *p;

	p = *hashtbl_getslot(h, key);
	return tblnode_keyexist(p, key);
}

int hashtbl_print_slot(int idx, struct tblnode *chain)
{
	pf("%d)\t", idx);
	tblnode_print(chain);
}

int hashtbl_print(struct hashtbl *h)
{
	int i;

	for (i = 0; i < h->bucketsize; i++)
		hashtbl_print_slot(i, *(h->bucket + i));

	return 1;
}

int hashtbl_size(struct hashtbl *h)
{
	return h->size;
}

int hashtbl_free(struct hashtbl *h)
{
	struct tblnode *p;
	int i;

	for (i = 0; p = *(h->bucket + i), i < h->bucketsize; i++)
		if (p)
			tblnode_free(p);

	free(h);

	return 1;
}
