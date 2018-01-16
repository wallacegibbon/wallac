#include <stdlib.h>
#include "misc.h"


struct listele { struct listele *next; void *value; };
struct list { struct listele *ele; int size; };

#define CT_CHAR 1
#define CT_UCHAR 2
#define CT_SHORT 3
#define CT_USHORT 4
#define CT_INT 5
#define CT_UINT 6
#define CT_LONG 7
#define CT_ULONG 8
#define CT_STRUCT 9
#define CT_FUNC 9
#define CT_VOID 10

struct ctype { int type;
  int structidx; struct ctype *fnret; struct list *fnparams; };

struct cfunc { char *name; struct ctype *ret;
  struct list *params, *vars, *stmts; };

struct cvar { char *name; struct ctype *type;
  int pdepth; int is_extern; };

struct sfield { char *name; struct ctype *type;
  int pdepth; };

struct cstruct { char *name; struct list *fields; };

struct stmt { int type; void *value; };


struct list *struct_defs;
struct list *global_vars;
struct list *functions;



struct ctype *
new_ctype(int type, int structidx, struct ctype *fnret, struct list *fnparams)
{
  struct ctype *t;
  t = malloc(sizeof(struct ctype));
  if (!t)
    exit_with("Failed alloc memory for new c type\n");

  t->type = type;
  t->structidx = structidx;
  t->fnret = fnret;
  t->fnparams = fnparams;
  return t;
}


struct cvar *
new_variable(char *name, struct ctype *type, int pdepth, int is_extern)
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


struct sfield *
new_field(char *name, struct ctype *type, int pdepth)
{
  struct sfield *f;
  f = malloc(sizeof(struct sfield));
  if (!f)
    exit_with("Failed alloc memory for new c struct field\n");

  f->name = name;
  f->type = type;
  f->pdepth = pdepth;
  return f;
}


struct cstruct *
new_struct(char *name)
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
new_function(char *name, struct list *params, struct list *vars,
    struct ctype *ret)
{
  struct cfunc *f;
  f = malloc(sizeof(struct cfunc));
  if (!f)
    exit_with("Failed alloc memory for new c function\n");

  f->name = name;
  f->ret = ret;
  f->params = params;
  f->vars = vars;
  f->stmts = NULL;
  return f;
}


