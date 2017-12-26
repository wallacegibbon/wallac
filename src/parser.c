#include <stdlib.h>
#include "lexer.h"
#include "misc.h"
#include "token.h"
#include "vars.h"



struct pair { struct pair *car, *cdr; int type; };

struct pair *program;


int
print_pairs(struct pair *p);


struct pair *
new_pair(int type, struct pair *car, struct pair *cdr)
{
  struct pair *p;

  p = malloc(sizeof(struct pair));
  if (!p)
    exit_with_info("Failed malloc memory for new pair\n");
  p->type = type;
  p->car = car;
  p->cdr = cdr;
  return p;
}


struct pair *
car(struct pair *p)
{
  return p->car;
}


struct pair *
cdr(struct pair *p)
{
  return p->cdr;
}


struct pair *
cons(struct pair *car, struct pair *cdr)
{
  struct pair *p;

  p = new_pair(0, car, cdr);
  return p;
}


int
print_pairs_nopa(struct pair *p)
{
  if (!p)
    return 0;

  if (p->type != 0)
    return printf(" %s ", token_type_str(p->type));

  if (car(p)->type != 0)
    print_pairs_nopa(car(p));
  else
    print_pairs(car(p));

  print_pairs_nopa(cdr(p));

  return 0;
}


int
print_pairs(struct pair *p)
{
  printf("(");
  print_pairs_nopa(p);
  printf(")\n");
  return 0;
}


struct pair *
new_token_pair(struct token *t)
{
  struct pair *p;

  p = new_pair(t->type, (struct pair *) t, NULL);
  return p;
}


int
check_type(int type)
{
  return type == KW_VOLATILE || type == KW_STATIC || type == KW_REGISTER ||
    type == KW_AUTO || type == KW_EXTERN || type == KW_CONST ||
    type == KW_INT || type == KW_SHORT || type == KW_LONG ||
    type == KW_CHAR || type == KW_VIOD;
}


struct token *
next_token()
{
  if (!current_tk)
    exit_with_info("No next token anymore\n");

  current_tk = current_tk->next;
  return current_tk;
}



struct pair *
collect_type_tokens()
{
  struct pair *type;

  type = NULL;
  for (; check_type(current_tk->type); next_token())
    type = cons(new_token_pair(current_tk), type);

  return type;
}


int
match_fn_declaration()
{
  struct pair *p;

  p = collect_type_tokens();
  if (!p)
    exit_with_info("%s:%d:[PARSER]Function declaration type error\n",
        filename, current_tk->line);

  if (current_tk->type != TK_IDENT)
    exit_with_info("%s:%d:[PARSER]Function declaration name error\n",
        filename, current_tk->line);

  next_token();
  if (current_tk->type != TK_OPENPA)
    exit_with_info("%s:%d:[PARSER]Function declaration ( error\n",
        filename, current_tk->line);

  return 0;
}


int
match_fn_definition()
{
  return 0;
}


int
match_struct_definition()
{
  return 0;
}


int
parse()
{
  struct pair *p;

  tokenize();
  print_token_list();

  current_tk = start_tk->next;

  //p = collect_type_tokens();
  //print_pairs(p);

  //match_fn_declaration();

  return 0;
}

