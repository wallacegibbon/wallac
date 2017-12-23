#include <string.h>
#include <stdio.h>


struct variable { char *name; int type; struct variable *next; };

struct variable *global_vars = NULL;



int
find_variable(char *name, struct variable *p, long *offset)
{
  long cnt = 0;

  for (; p && strcmp(p->name, name) != 0; cnt++)
    p = p->next;

  if (!p)
    return 0;

  *offset = cnt;
  return 1;
}
