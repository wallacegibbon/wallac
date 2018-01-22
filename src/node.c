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


int
compare_struct_name(char *n1, char *n2)
{
  if (!n1 && !n2)
    return 0;

  if (n1 && !n2)
    return 1;

  if (!n1 && n2)
    return -1;

  return scmp(n1, n2);
}


int
ctype_cmp(struct ctype *ct1, struct ctype *ct2)
{
  pf("-----------------\n");
  pf("t1: %d, t2: %d\n", ct1->type, ct2->type);
  pf("t1: %d, t2: %d\n", ct1->pdepth, ct2->pdepth);
  pf("t1: %d, t2: %d\n", ct1->is_extern, ct2->is_extern);
  pf("t1: %s, t2: %s\n", ct1->struct_name, ct2->struct_name);
  pf("-----------------\n");
  return ct1->type == ct2->type && ct1->pdepth == ct2->pdepth &&
    ct1->is_extern == ct2->is_extern &&
    !compare_struct_name(ct1->struct_name, ct2->struct_name);
}


int
ctype_free(struct ctype *ct)
{
  free(ct);
  return 1;
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


int
cvar_free(struct cvar *cv)
{
  ctype_free(cv->type);
  free(cv);

  return 1;
}


struct cstruct *
new_cstruct(char *name, struct linktbl *fields)
{
  struct cstruct *s;
  s = malloc(sizeof(struct cstruct));
  if (!s)
    exit_with("Failed alloc memory for new c struct\n");

  if (!fields)
    fields = new_linktbl();

  s->name = name;
  s->fields = fields;
  return s;
}


struct cfunc *
new_cfunc(char *name, struct ctype *ret, struct linktbl *params,
    struct linktbl *vars, struct linktbl *stmts, int var_arg)
{
  struct cfunc *f;
  f = malloc(sizeof(struct cfunc));
  if (!f)
    exit_with("Failed alloc memory for new c function\n");

  if (!params)
    params = new_linktbl();

  if (!vars)
    vars = new_linktbl();

  if (!stmts)
    stmts = new_linktbl();

  f->name = name;
  f->ret = ret;
  f->params = params;
  f->vars = vars;
  f->stmts = stmts;
  f->var_arg = var_arg;
  f->is_declare = 0;
  return f;
}


