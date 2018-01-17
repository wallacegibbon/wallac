#ifndef __LINKTBL_H__
#define __LINKTBL_H__


#include "tblnode.h"


struct linktbl { struct tblnode *chain; int size; };


struct linktbl *
new_linktbl();

int
linktbl_put(struct linktbl *l, char *key, void *value);

struct tblnode *
linktbl_get(struct linktbl *l, char *key);

int
linktbl_keyexist(struct linktbl *l, char *key);

int
linktbl_print(struct linktbl *l);

int
linktbl_size(struct linktbl *l);

int
linktbl_free(struct linktbl *l);


#endif
