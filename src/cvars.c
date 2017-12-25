#include <string.h>
#include <stdio.h>
#include "cvars.h"
#include "limits.h"


struct cvars *global_vars = NULL;
struct cstruct *struct_defs = NULL;



int
find_variable(char *name, struct cvars *p, long *offset)
{
  long cnt = 0;

  for (; p && strcmp(p->name, name) != 0; p = p->next)
    cnt++;

  if (!p)
    return 0;

  *offset = cnt;
  return 1;
}


int
sizeof_type(int type)
{
  if (is_pointer(type))
    return LONG_BIT / CHAR_BIT;
  if (is_int_long(type))
    return LONG_BIT / CHAR_BIT;
  if (is_int_int(type))
    return INT_BIT / CHAR_BIT;
  if (is_int_short(type))
    return SHORT_BIT / CHAR_BIT;
  if (is_char(type))
    return 1;
  if (is_struct(type))
    return sizeof_struct(type);

  return 0;
}


int
sizeof_struct(int type)
{
  int idx = (type >> STRUCTIDX_STARTBIT) & 0xff;
  struct cstruct *p = struct_defs;

  while (idx--)
    p = p->next;

  return sizeof_vars(p->vars);
}


int
sizeof_vars(struct cvars *p)
{
  int len = 0;
  for (; p; p = p->next)
    len += sizeof_type(p->type);

  return len;
}


int
is_pointer(int type)
{
  return type & POINTER_MASK == TYPE_POINTER;
}


int
is_void(int type)
{
  return type & TYPE_MASK == TYPE_VOID;
}


int
is_char(int type)
{
  return type & TYPE_MASK == TYPE_CHAR;
}


int
is_int(int type)
{
  return type & TYPE_MASK == TYPE_INT;
}


int
is_int_int(int type)
{
  return is_int(type) && (type & INTLEN_MASK == 0);
}


int
is_int_short(int type)
{
  return is_int(type) && (type & INTLEN_MASK == INT_SHORT);
}


int
is_int_long(int type)
{
  return is_int(type) && (type & INTLEN_MASK == INT_LONG);
}


int
is_int_unsigned(int type)
{
  return is_int(type) && (type & INTSIGN_MASK == INT_UNSIGNED);
}


int
is_struct(int type)
{
  return type & TYPE_MASK == TYPE_STRUCT;
}


int
is_extern(int type)
{
  return type & STORAGE_MASK == STORAGE_EXTERN;
}

