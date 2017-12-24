#include <string.h>
#include <stdio.h>
#include "cvars.h"


struct cvars *global_vars = NULL;



int
find_variable(char *name, struct cvars *p, long *offset)
{
  long cnt = 0;

  for (; p && strcmp(p->name, name) != 0; cnt++)
    p = p->next;

  if (!p)
    return 0;

  *offset = cnt;
  return 1;
}
