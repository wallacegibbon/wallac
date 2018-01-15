#ifndef __HASHTBL_H__
#define __HASHTBL_H__


struct hashnode { struct hashnode *next; char *key; void *value; };
struct hashtbl { struct hashnode **bucket; int bucketsize; };


struct hashtbl *
new_hashtbl(int bucketsize);

int
free_hashtbl(struct hashtbl *h);

int
hash_keyexist(struct hashtbl *h, char *key);

int
hash_put(struct hashtbl *h, char *key, void *value);

struct hashnode *
hash_get(struct hashtbl *h, char *key);

int
hash_print(struct hashtbl *h);


#endif

