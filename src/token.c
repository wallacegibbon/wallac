#include <stdlib.h>
#include "token.h"
#include "misc.h"



struct token *
new_token(short type, void *value)
{
  struct token *t = malloc(sizeof(struct token));
  if (!t)
    exit_with_info("Failed alloc memory for token %d\n", type);

  t->type = type;
  t->value = value;

  return t;
}

