#ifndef __HASHTBL_H__
#define __HASHTBL_H__


struct hashnode { struct hashnode *next; char *key; void *value; };
struct hashtbl { struct hashnode **bucket; int bucketsize; };


struct hashtbl *
new_hashtbl(int bucketsize);

int
hash_keyexist(struct hashtbl *h, char *key);

int
hash_insert(struct hashtbl *h, char *key, void *value);

int
hash_print(struct hashtbl *h);


#endif

