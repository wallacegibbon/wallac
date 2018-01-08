#include <stdlib.h>
#include "misc.h"


struct cfunc { char *name; struct list *params, *vars; int type; };
struct cvar { int type; char *name; int is_extern; };
struct sfield { int type; char *name; };
struct cstruct { char *name; struct list *fields; int size; };

struct list { struct varlist *next; void *value; };

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
  s->size = 0;
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
  f->params = params;
  f->vars = vars;
  f->type = type;
  return f;
}


