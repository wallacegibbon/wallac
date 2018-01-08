#include <stdlib.h>
#include "misc.h"


struct listele { struct listele *next; void *value; };
struct list { struct listele *ele; int size; };

struct cfunc { char *name; int type; struct list *params, *vars, *stmts; };
struct cvar { char *name; int type; int is_extern; };
struct sfield { char *name; int type; };
struct cstruct { char *name; struct list *fields; };

struct stmt { int type; void *value; };

struct list *struct_defs;
struct list *global_vars;
struct list *functions;



struct cvar *
new_variable(char *name, int type, int is_extern)
{
  struct cvar *v;
  v = malloc(sizeof(struct cvar));
  if (!v)
    exit_with_info("Failed alloc memory for new c variable\n");

  v->name = name;
  v->type = type;
  v->is_extern = is_extern;
  return v;
}


struct sfield *
new_field(char *name, int type)
{
  struct sfield *f;
  f = malloc(sizeof(struct sfield));
  if (!f)
    exit_with_info("Failed alloc memory for new struct field\n");

  f->name = name;
  f->type = type;
  return f;
}


struct cstruct *
new_struct(char *name)
{
  struct cstruct *s;
  s = malloc(sizeof(struct cstruct));
  if (!s)
    exit_with_info("Failed alloc memory for new c struct\n");

  s->name = name;
  s->fields = NULL;
  return s;
}


struct cfunc *
new_function(char *name, struct list *params, struct list *vars, int type)
{
  struct cfunc *f;
  f = malloc(sizeof(struct cfunc));
  if (!f)
    exit_with_info("Failed alloc memory for new c function\n");

  f->name = name;
  f->type = type;
  f->params = params;
  f->vars = vars;
  f->stmts = NULL;
  return f;
}


