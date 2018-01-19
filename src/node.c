#include <stdlib.h>
#include "misc.h"
#include "node.h"



struct ctype *
new_ctype(int type, char *struct_name)
{
  struct ctype *t;
  t = malloc(sizeof(struct ctype));
  if (!t)
    exit_with("Failed alloc memory for new c type\n");

  t->type = type;
  t->struct_name = struct_name;
  return t;
}


struct cvar *
new_cvar(char *name, struct ctype *type, int pdepth, int is_extern)
{
  struct cvar *v;
  v = malloc(sizeof(struct cvar));
  if (!v)
    exit_with("Failed alloc memory for new c variable\n");

  v->name = name;
  v->type = type;
  v->pdepth = pdepth;
  v->is_extern = is_extern;
  return v;
}


struct cstruct *
new_cstruct(char *name)
{
  struct cstruct *s;
  s = malloc(sizeof(struct cstruct));
  if (!s)
    exit_with("Failed alloc memory for new c struct\n");

  s->name = name;
  s->fields = NULL;
  return s;
}


struct cfunc *
new_cfunc(char *name, struct ctype *ret)
{
  struct cfunc *f;
  f = malloc(sizeof(struct cfunc));
  if (!f)
    exit_with("Failed alloc memory for new c function\n");

  f->name = name;
  f->ret = ret;
  f->vars = NULL;
  f->stmts = NULL;
  return f;
}


