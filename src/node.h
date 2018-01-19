#ifndef __NODE_H__
#define __NODE_H__


#include "linktbl.h"


struct ctype { int type; int structidx; };

struct cvar { char *name; struct ctype *type; int pdepth; int is_extern; };
struct cstruct { char *name; struct linktbl *fields; };
struct cfunc { char *name; struct ctype *ret;
  struct linktbl *params, *vars, *stmts; };

struct stmt { int type; void *value; };


struct ctype *
new_ctype(int type, int structidx);

struct cvar *
new_cvar(char *name, struct ctype *type, int pdepth, int is_extern);

struct cstruct *
new_cstruct(char *name);

struct cfunc *
new_cfunc(char *name, struct ctype *ret);


#endif
