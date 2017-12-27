#include <stdlib.h>
#include "lexer.h"
#include "misc.h"
#include "token.h"
#include "cvars.h"
#include "vars.h"


int
parse()
{
  struct pair *p;

  tokenize();
  print_token_list();

  current_tk = start_tk->next;

  if (!current_tk)
    exit_with_info("NO token error\n");

  handle();
  //p = collect_type_tokens();
  //print_pairs(p);

  //match_fn_declaration();

  return 0;
}


int
next_token_noend()
{
  struct token *prev;
  prev = current_tk;
  current_tk = current_tk->next;
  if (!current_tk)
    exit_with_info("%s:%d:[PARSER]Unexpected end of file\n",
        filename, prev->line);
  return current_tk;
}

int
next_token()
{
  current_tk = current_tk->next;
  return current_tk;
}

int
simple_type(int type)
{
  return type == KW_VOLATILE || type == KW_STATIC || type == KW_REGISTER ||
    type == KW_AUTO || type == KW_EXTERN || type == KW_CONST ||
    type == KW_INT || type == KW_SHORT || type == KW_LONG ||
    type == KW_CHAR || type == KW_VIOD;
}


int
handle()
{
  if (current_tk->type == KW_STRUCT)
    return handle_struct();
  if (simple_type(current_tk->type))
    return handle_global_var();
  return 1;
}


int
handle_struct()
{
  char *struct_name, *var_name;
  int type;

  next_token_noend();
  if (current_tk->type != TK_IDENT)
    exit_with_info("%s:%d:[PARSER]Syntax error near struct\n",
        filename, current_tk->line);

  struct_name = (char *) current_tk->value;

  next_token_noend();
  if (current_tk->type == TK_BEGIN)
    return handle_struct_definition(struct_name);

  if (current_tk->type != TK_IDENT)
    exit_with_info("%s:%d:[PARSER]Syntax error near struct\n",
        filename, current_tk->line);

  var_name = (char *) current_tk->value;
  type = get_struct_type(struct_name);
  add_global_variable(var_name, var_type);

  return 1;
}

int
handle_struct_definition(char *name)
{
}


int
get_struct_type(char *name)
{
}


int
add_global_variable(char *name, char *type)
{
}


int
add_type_attr(int *type, int new_attr, int mask)
{
  if (*type & mask != 0)
    exit_with_info("%s:%d:[PARSER]Syntax error on variable definition\n",
        filename, current_tk->line);
  *type |= new_attr;
  return 1;
}

int
handle_global_var()
{
  int type = 0;
  if (current_tk->type == KW_VIOD)
    add_type_attr(&type, TYPE_VOID, TYPE_MASK);
  if (current_tk->type == KW_CHAR)
    add_type_attr(&type, TYPE_CHAR, TYPE_MASK);
  if (current_tk->type == KW_INT)
    add_type_attr(&type, TYPE_INT, TYPE_MASK);
  if (current_tk->type == KW_SHORT)
    add_type_attr(&type, INT_SHORT, INTLEN_MASK);
  if (current_tk->type == KW_LONG)
    add_type_attr(&type, INT_LONG, INTLEN_MASK);
  if (current_tk->type == KW_UNSIGNED)
    add_type_attr(&type, INT_UNSIGNED, INTSIGN_MASK);
  if (current_tk->type == KW_SIGNED)
    add_type_attr(&type, INT_SIGNED, INTSIGN_MASK);
  if (current_tk->type == TK_ASTERISK)
    type |= POINTER_MASK;

}


int
collect_non_pointer_type_attr()
{
}
