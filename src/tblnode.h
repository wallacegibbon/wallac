#ifndef __TBLNODE_H__
#define __TBLNODE_H__

struct tblnode {
	struct tblnode *next;
	char *key;
	void *value;
};

struct tblnode *new_tblnode(char *key, void *value);

int tblnode_add(struct tblnode *n, char *key, void *value);

int tblnode_set(struct tblnode *n, char *key, void *value);

void *tblnode_get(struct tblnode *n, char *key);

int tblnode_keyexist(struct tblnode *n, char *key);

int tblnode_print(struct tblnode *p);

int tblnode_free(struct tblnode *s);

#endif
