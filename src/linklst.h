#ifndef __LINKLST_H__
#define __LINKLST_H__



struct lstnode { struct lstnode *prev, *next; void *value; };
struct linklst { struct lstnode *chain, *cursor; int size; };


struct linklst *
new_linklst();

int
linklst_push(struct linklst *lst, void *value);

void *
linklst_pop(struct linklst *lst);

int
linklst_print(struct linklst *l);

#endif
