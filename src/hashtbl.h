#ifndef __HASHTBL_H__
#define __HASHTBL_H__


#include "tblnode.h"


struct hashtbl { struct tblnode **bucket; int bucketsize, size; };


struct hashtbl *
new_hashtbl(int bucketsize);

int
hashtbl_put(struct hashtbl *h, char *key, void *value);

struct tblnode *
hashtbl_get(struct hashtbl *h, char *key);

int
hashtbl_keyexist(struct hashtbl *h, char *key);

int
hashtbl_size(struct hashtbl *h);

int
hashtbl_print(struct hashtbl *h);

int
hashtbl_free(struct hashtbl *h);


#endif

