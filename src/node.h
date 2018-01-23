#ifndef __NODE_H__
#define __NODE_H__


#include "linktbl.h"


struct ctype { int type, pdepth; char *struct_name; int is_extern; };
struct cvar { char *name; struct ctype *type; };

struct cstruct { char *name; struct linktbl *fields; };
struct cfunc { char *name; struct ctype *ret;
  struct linktbl *params, *vars, *stmts; int var_arg, is_declare; };


struct ctype *
new_ctype(int type, int pdepth, char *struct_name, int is_extern);

struct ctype *
ctype_copy(struct ctype *ct);

int
ctype_cmp(struct ctype *ct1, struct ctype *ct2);

int
ctype_free(struct ctype *ct);

struct cvar *
new_cvar(char *name, struct ctype *type);

int
cvar_free(struct cvar *cv);

struct cstruct *
new_cstruct(char *name, struct linktbl *fields);

struct cfunc *
new_cfunc(char *name, struct ctype *ret, struct linktbl *params,
    struct linktbl *vars, struct linktbl *stmts, int var_arg);


#endif
