#include <stdlib.h>
#include "token.h"
#include "misc.h"


struct token *start_tk = NULL, *current_tk = NULL;


void
initialize_token_list()
{
  start_tk = malloc(sizeof(struct token));
  if (!start_tk)
    exit_with_info("Failed alloc memory for start_tk\n");

  start_tk->type = 0;
  start_tk->l = NULL;
  start_tk->r = NULL;
  current_tk = start_tk;
}


void
join_token(short type, void *p)
{
  struct token *t = new_token(type, p);
  t->l = current_tk;
  t->r = NULL;
  current_tk->r = t;
  current_tk = t;
}


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


void
print_token_list()
{
  struct token *p;
  p = start_tk->r;

  while (p)
    print_token(slide_tk(&p));
}


void
print_token_type(short type)
{
  switch (type)
  {
  case TK_ASTERISK:
    printf("[TK_ASTERISK]");
    break;
  case TK_DPLUS:
    printf("[TK_DPLUS]");
    break;
  case TK_PLUS:
    printf("[TK_PLUS]");
    break;
  case TK_DMINUS:
    printf("[TK_DMINUS]");
    break;
  case TK_MINUS:
    printf("[TK_MINUS]");
    break;
  case TK_POINTSTO:
    printf("[TK_POINTSTO]");
    break;
  case TK_DIVIDE:
    printf("[TK_DIVIDE]");
    break;
  case TK_MOD:
    printf("[TK_MOD]");
    break;
  case TK_ASSIGN:
    printf("[TK_ASSIGN]");
    break;
  case TK_EQ:
    printf("[TK_EQ]");
    break;
  case TK_EXCLAMATION:
    printf("[TK_EXCLAMATION]");
    break;
  case TK_NEQ:
    printf("[TK_NEQ]");
    break;
  case TK_LT:
    printf("[TK_LT]");
    break;
  case TK_LEQ:
    printf("[TK_LEQ]");
    break;
  case TK_GT:
    printf("[TK_GT]");
    break;
  case TK_GEQ:
    printf("[TK_GEQ]");
    break;
  case TK_DOT:
    printf("[TK_DOT]");
    break;
  case TK_ELLIPSIS:
    printf("[TK_ELLIPSIS]");
    break;
  case TK_TILDE:
    printf("[TK_TILDE]");
    break;
  case TK_CARET:
    printf("[TK_CARET]");
    break;
  case TK_AND:
    printf("[TK_AND]");
    break;
  case TK_OR:
    printf("[TK_OR]");
    break;
  case TK_DAND:
    printf("[TK_DAND]");
    break;
  case TK_DOR:
    printf("[TK_DOR]");
    break;
  case TK_OPENPA:
    printf("[TK_OPENPA]");
    break;
  case TK_CLOSEPA:
    printf("[TK_CLOSEPA]");
    break;
  case TK_OPENBR:
    printf("[TK_OPENBR]");
    break;
  case TK_CLOSEBR:
    printf("[TK_CLOSEBR]");
    break;
  case TK_BEGIN:
    printf("[TK_BEGIN]");
    break;
  case TK_END:
    printf("[TK_END]");
    break;
  case TK_QUESTION:
    printf("[TK_QUESTION]");
    break;
  case TK_COLON:
    printf("[TK_COLON]");
    break;
  case TK_SEMICOLON:
    printf("[TK_SEMICOLON]");
    break;
  case TK_COMMA:
    printf("[TK_COMMA]");
    break;
  case TK_CINT:
    printf("[TK_CINT]");
    break;
  case TK_CCHAR:
    printf("[TK_CCHAR]");
    break;
  case TK_CSTR:
    printf("[TK_CSTR]");
    break;
  case TK_IDENT:
    printf("[TK_IDENT]");
    break;
  case TK_EOF:
    printf("[TK_EOF]");
    break;
  }
}


void
print_token(struct token *t)
{
  print_token_type(t->type);
  //printf("[%d]", t->type);
  if (t->type == TK_CSTR)
    printf("\"%s\"\n", t->value);
  else if (t->type == TK_IDENT)
    printf("<%s>\n", t->value);
  else
    printf("0x%x, 0o%o, %d\n", t->value, t->value, t->value);
}


struct token *
slide_tk(struct token **p)
{
  struct token *t = *p;
  *p = (*p)->r;
  return t;
}


