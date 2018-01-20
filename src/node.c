#include <stdlib.h>
#include "misc.h"
#include "node.h"
#include "ctypes.h"
#include "libc.h"



struct ctype *
new_ctype(int type, int pdepth, char *struct_name, int is_extern)
{
  struct ctype *t;
  t = malloc(sizeof(struct ctype));
  if (!t)
    exit_with("Failed alloc memory for new c type\n");

  t->type = type;
  t->pdepth = pdepth;
  t->struct_name = struct_name;
  t->is_extern = is_extern;
  return t;
}


struct ctype *
ctype_copy(struct ctype *ct)
{
  struct ctype *t;
  t = new_ctype(ct->type, ct->pdepth, ct->struct_name, ct->is_extern);
  return t;
}


struct cvar *
new_cvar(char *name, struct ctype *type)
{
  struct cvar *v;
  v = malloc(sizeof(struct cvar));
  if (!v)
    exit_with("Failed alloc memory for new c variable\n");

  v->name = name;
  v->type = type;
  return v;
}


struct cstruct *
new_cstruct(char *name, struct linktbl *fields)
{
  struct cstruct *s;
  s = malloc(sizeof(struct cstruct));
  if (!s)
    exit_with("Failed alloc memory for new c struct\n");

  s->name = name;
  s->fields = fields;
  return s;
}


struct cfunc *
new_cfunc(char *name, struct ctype *ret, struct linktbl *params)
{
  struct cfunc *f;
  f = malloc(sizeof(struct cfunc));
  if (!f)
    exit_with("Failed alloc memory for new c function\n");

  f->name = name;
  f->ret = ret;
  f->params = params;
  f->vars = NULL;
  f->stmts = NULL;
  f->defined = 0;
  return f;
}


