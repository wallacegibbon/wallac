#ifndef __CVARIABLE_H__
#define __CVARIABLE_H__


struct cvars { char *name; int type; struct cvars *next; };

int
find_variable(char *name, struct cvars *p, long *offset);


#endif
