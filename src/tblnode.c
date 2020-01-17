#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tblnode.h"
#include "misc.h"

struct tblnode *new_tblnode(char *key, void *value)
{
	struct tblnode *n;

	n = malloc(sizeof(struct tblnode));
	if (!n)
		exit_with("Failed alloc memory for table node\n");

	n->key = key;
	n->value = value;
	n->next = NULL;

	return n;
}

int tblnode_add(struct tblnode *n, char *key, void *value)
{
	struct tblnode *t;

	if (!n)
		return 0;

	for (; n && strcmp(n->key, key); n = n->next)
		t = n;

	if (n)
		return 0;

	t->next = new_tblnode(key, value);

	return 1;
}

int tblnode_set(struct tblnode *n, char *key, void *value)
{
	struct tblnode *t;

	if (!n)
		return 0;

	for (; n && strcmp(n->key, key);)
		n = n->next;

	if (!n)
		return 0;

	n->value = value;

	return 1;
}

void *tblnode_get(struct tblnode *n, char *key)
{
	if (!n)
		return NULL;

	for (; n && strcmp(n->key, key);)
		n = n->next;

	if (n)
		return n->value;
	else
		return NULL;
}

int tblnode_keyexist(struct tblnode *n, char *key)
{
	return tblnode_get(n, key) != NULL;
}

int tblnode_print(struct tblnode *n)
{
	for (; n; n = n->next)
		printf("<%s|%d>--", n->key, n->value);

	printf("\n");

	return 1;
}

int tblnode_free(struct tblnode *n)
{
	struct tblnode *p;

	if (!n)
		return 1;

	for (; p = n->next, p; n = p)
		free(n);

	free(n);

	return 1;
}
