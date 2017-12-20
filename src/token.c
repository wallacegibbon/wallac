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
join_token(short type, void *p)
{
  struct token *t = new_token(type, p);
  t->l = current_tk;
  t->r = NULL;
  current_tk->r = t;
  current_tk = t;
}


void
print_token_list()
{
  struct token *p;
  p = start_tk->r;

  while (p)
    print_token(slide_tk(&p));
}


struct token *
slide_tk(struct token **p)
{
  struct token *t = *p;
  *p = (*p)->r;
  return t;
}


void
print_token(struct token *t)
{
  printf("[%s]", token_type_str(t->type));
  if (t->type != TK_CSTR && t->type != TK_IDENT)
    printf("0x%x, 0o%o, %d\n", t->value, t->value, t->value);
  else
    printf("<%s>\n", t->value);
}


char *
token_type_str(short type)
{
  switch (type)
  {
  case TK_ASTERISK:
    return "TK_ASTERISK";
  case TK_DPLUS:
    return "TK_DPLUS";
  case TK_PLUS:
    return "TK_PLUS";
  case TK_DMINUS:
    return "TK_DMINUS";
  case TK_MINUS:
    return "TK_MINUS";
  case TK_POINTSTO:
    return "TK_POINTSTO";
  case TK_DIVIDE:
    return "TK_DIVIDE";
  case TK_MOD:
    return "TK_MOD";
  case TK_ASSIGN:
    return "TK_ASSIGN";
  case TK_EQ:
    return "TK_EQ";
  case TK_EXCLAMATION:
    return "TK_EXCLAMATION";
  case TK_NEQ:
    return "TK_NEQ";
  case TK_LT:
    return "TK_LT";
  case TK_LEQ:
    return "TK_LEQ";
  case TK_GT:
    return "TK_GT";
  case TK_GEQ:
    return "TK_GEQ";
  case TK_DOT:
    return "TK_DOT";
  case TK_ELLIPSIS:
    return "TK_ELLIPSIS";
  case TK_TILDE:
    return "TK_TILDE";
  case TK_CARET:
    return "TK_CARET";
  case TK_AND:
    return "TK_AND";
  case TK_OR:
    return "TK_OR";
  case TK_DAND:
    return "TK_DAND";
  case TK_DOR:
    return "TK_DOR";
  case TK_OPENPA:
    return "TK_OPENPA";
  case TK_CLOSEPA:
    return "TK_CLOSEPA";
  case TK_OPENBR:
    return "TK_OPENBR";
  case TK_CLOSEBR:
    return "TK_CLOSEBR";
  case TK_BEGIN:
    return "TK_BEGIN";
  case TK_END:
    return "TK_END";
  case TK_QUESTION:
    return "TK_QUESTION";
  case TK_COLON:
    return "TK_COLON";
  case TK_SEMICOLON:
    return "TK_SEMICOLON";
  case TK_COMMA:
    return "TK_COMMA";
  case TK_CINT:
    return "TK_CINT";
  case TK_CCHAR:
    return "TK_CCHAR";
  case TK_CSTR:
    return "TK_CSTR";
  case TK_IDENT:
    return "TK_IDENT";
  case TK_EOF:
    return "TK_EOF";
  default:
    return "TK_UNKNOWN";
  }
}


