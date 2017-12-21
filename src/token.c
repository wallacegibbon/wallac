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
new_token(int type, void *value)
{
  struct token *t = malloc(sizeof(struct token));
  if (!t)
    exit_with_info("Failed alloc memory for token %d\n", type);

  t->type = type;
  t->value = value;

  return t;
}


void
join_token(int line, int type, void *p)
{
  struct token *t = new_token(type, p);
  t->line = line;
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
  printf("(%d)[%s]", t->line, token_type_str(t->type));
  if (t->type != TK_CSTR && t->type != TK_IDENT)
    printf("0x%x, 0o%o, %d\n", t->value, t->value, t->value);
  else
    printf("<%s>\n", t->value);
}


char *
token_type_str(int type)
{
  if (type == TK_ASTERISK)
    return "TK_ASTERISK";
  if (type == TK_DPLUS)
    return "TK_DPLUS";
  if (type == TK_PLUS)
    return "TK_PLUS";
  if (type == TK_DMINUS)
    return "TK_DMINUS";
  if (type == TK_MINUS)
    return "TK_MINUS";
  if (type == TK_POINTSTO)
    return "TK_POINTSTO";
  if (type == TK_DIVIDE)
    return "TK_DIVIDE";
  if (type == TK_MOD)
    return "TK_MOD";
  if (type == TK_ASSIGN)
    return "TK_ASSIGN";
  if (type == TK_EQ)
    return "TK_EQ";
  if (type == TK_EXCLAMATION)
    return "TK_EXCLAMATION";
  if (type == TK_NEQ)
    return "TK_NEQ";
  if (type == TK_LT)
    return "TK_LT";
  if (type == TK_LEQ)
    return "TK_LEQ";
  if (type == TK_GT)
    return "TK_GT";
  if (type == TK_GEQ)
    return "TK_GEQ";
  if (type == TK_DOT)
    return "TK_DOT";
  if (type == TK_ELLIPSIS)
    return "TK_ELLIPSIS";
  if (type == TK_TILDE)
    return "TK_TILDE";
  if (type == TK_CARET)
    return "TK_CARET";
  if (type == TK_AND)
    return "TK_AND";
  if (type == TK_OR)
    return "TK_OR";
  if (type == TK_DAND)
    return "TK_DAND";
  if (type == TK_DOR)
    return "TK_DOR";
  if (type == TK_OPENPA)
    return "TK_OPENPA";
  if (type == TK_CLOSEPA)
    return "TK_CLOSEPA";
  if (type == TK_OPENBR)
    return "TK_OPENBR";
  if (type == TK_CLOSEBR)
    return "TK_CLOSEBR";
  if (type == TK_BEGIN)
    return "TK_BEGIN";
  if (type == TK_END)
    return "TK_END";
  if (type == TK_QUESTION)
    return "TK_QUESTION";
  if (type == TK_COLON)
    return "TK_COLON";
  if (type == TK_SEMICOLON)
    return "TK_SEMICOLON";
  if (type == TK_COMMA)
    return "TK_COMMA";
  if (type == TK_CINT)
    return "TK_CINT";
  if (type == TK_CCHAR)
    return "TK_CCHAR";
  if (type == TK_CSTR)
    return "TK_CSTR";
  if (type == TK_IDENT)
    return "TK_IDENT";
  if (type == TK_EOF)
    return "TK_EOF";

  return "TK_UNKNOWN";
}


