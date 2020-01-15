#include <stdlib.h>
#include "misc.h"
#include "limits.h"
#include "linktbl.h"
#include "libc.h"

struct linktbl *new_linktbl()
{
	struct linktbl *p;

	p = malloc(sizeof(struct linktbl));
	if (!p)
		exit_with("Failed alloc memory for link table\n");

	p->size = 0;
	p->chain = NULL;

	return p;
}

int linktbl_increase_size(struct linktbl *l)
{
	if (l->size == INT_MAX)
		exit_with("linktbl size too big\n");

	l->size += 1;
	return 1;
}

int linktbl_add(struct linktbl *l, char *key, void *value)
{
	int r;

	r = 1;
	if (l->chain)
		r = tblnode_add(l->chain, key, value);
	else
		l->chain = new_tblnode(key, value);

	if (r > 0)
		linktbl_increase_size(l);

	return r;
}

int linktbl_set(struct linktbl *l, char *key, void *value)
{
	int r;

	if (!l->chain)
		return 0;

	r = tblnode_set(l->chain, key, value);

	return r;
}

void *linktbl_get(struct linktbl *l, char *key)
{
	return tblnode_get(l->chain, key);
}

void *linktbl_getidx(struct linktbl *l, int idx)
{
	struct tblnode *n;
	int cnt;

	if (idx >= l->size)
		return NULL;

	n = l->chain;
	for (cnt = 0; cnt < idx; cnt++)
		n = n->next;

	return n->value;
}

int linktbl_idxof(struct linktbl *l, char *key)
{
	struct tblnode *n;
	int idx;

	n = l->chain;
	idx = 0;
	for (; n && scmp(n->key, key); n = n->next)
		idx++;

	if (n)
		return idx;
	else
		return -1;
}

int linktbl_keyexist(struct linktbl *l, char *key)
{
	return tblnode_keyexist(l->chain, key);
}

int linktbl_print(struct linktbl *l)
{
	tblnode_print(l->chain);

	return 1;
}

int linktbl_size(struct linktbl *l)
{
	return l->size;
}

int linktbl_free(struct linktbl *l)
{
	tblnode_free(l->chain);
	free(l);

	return 1;
}
