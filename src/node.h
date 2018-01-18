#ifndef __NODE_H__
#define __NODE_H__


#include "linktbl.h"


struct ctype { int type;
  int structidx; struct ctype *fnret; struct linktbl *fnparams; };

struct cvar { char *name; struct ctype *type; int pdepth; int is_extern; };
struct cstruct { char *name; struct linktbl *fields; };
struct cfunc { char *name; struct ctype *type; struct linktbl *vars, *stmts; };

struct stmt { int type; void *value; };


struct ctype *
new_ctype(int type, int structidx, struct ctype *fnret,
    struct linktbl *fnparams);

struct cvar *
new_cvar(char *name, struct ctype *type, int pdepth, int is_extern);

struct cstruct *
new_cstruct(char *name);

struct cfunc *
new_cfunc(char *name, struct ctype *type, struct linktbl *vars);


#endif
