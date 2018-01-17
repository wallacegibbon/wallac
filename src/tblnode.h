#ifndef __TBLNODE_H__
#define __TBLNODE_H__


struct tblnode { struct tblnode *next; char *key; void *value; };


struct tblnode *
new_tblnode(char *key, void *value);

int
tblnode_append(struct tblnode *p, char *key, void *value);

struct tblnode *
tblnode_get(struct tblnode *n, char *key);

int
tblnode_keyexist(struct tblnode *n, char *key);

int
tblnode_print(struct tblnode *p);

int
tblnode_free(struct tblnode *s);


#endif
