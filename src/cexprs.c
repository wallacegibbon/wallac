#include <stdlib.h>
#include "cexprs.h"
#include "misc.h"


struct expr *
new_expr(int type)
{
  struct expr *e;

  e = malloc(sizeof(struct expr));
  if (!e)
    exit_with("Failed alloc memory for new expression\n");

  e->type = type;
  e->op1 = NULL;
  e->op2 = NULL;

  return e;
}

