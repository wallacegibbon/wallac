#include <stdlib.h>
#include "token.h"
#include "misc.h"
#include "libc.h"



struct token *
new_token(int type, void *value)
{
  struct token *t;

  t = malloc(sizeof(struct token));
  if (!t)
    exit_with("Failed alloc memory for token %d\n", type);

  t->prev = NULL;
  t->next = NULL;

  t->type = type;
  t->value = value;

  t->fname = NULL;
  t->line = 0;

  return t;
}


struct token *
copy_token(struct token *chain, struct token *orig)
{
  struct token *t;

  t = new_token(orig->type, orig->value);

  t->fname = orig->fname;
  t->line = orig->line;

  t->prev = chain;
  chain->next = t;

  return chain->next;
}


struct token *
copy_token_chain(struct token *orig)
{
  struct token *r, *t;

  if (!orig)
    return NULL;

  t = new_token(0, NULL);
  r = t;
  for (; orig; orig = orig->next)
    r = copy_token(r, orig);

  r = t->next;
  free(t);

  return r;
}


void
print_token_value_as_int(void *raw)
{
  int v;
  v = (int) raw;
  pf("0x%x, 0o%o, %d\n", v, v, v);
}


int
print_token(struct token *t)
{
  pf("(%s:%d)%s ", t->fname, t->line, token_type_str(t->type));
  if (t->type != TK_CSTR && t->type != TK_IDENT)
    print_token_value_as_int(t->value);
  else
    pf("<%s>\n", t->value);

  return 1;
}


int
print_token_list(struct token *start)
{
  struct token *p;

  for (p = start; p; p = p->next)
    print_token(p);

  pf("\n");
  return 1;
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
  if (type == KW_IF)
    return "if";
  if (type == KW_ELSE)
    return "else";
  if (type == KW_WHILE)
    return "while";
  if (type == KW_SWITCH)
    return "switch";
  if (type == KW_CASE)
    return "case";
  if (type == KW_DEFAULT)
    return "default";
  if (type == KW_FOR)
    return "for";
  if (type == KW_DO)
    return "do";
  if (type == KW_RETURN)
    return "return";
  if (type == KW_GOTO)
    return "goto";
  if (type == KW_CONTINUE)
    return "continue";
  if (type == KW_BREAK)
    return "break";
  if (type == KW_VIOD)
    return "void";
  if (type == KW_CHAR)
    return "char";
  if (type == KW_INT)
    return "int";
  if (type == KW_FLOAT)
    return "float";
  if (type == KW_DOUBLE)
    return "double";
  if (type == KW_SHORT)
    return "short";
  if (type == KW_LONG)
    return "long";
  if (type == KW_SIGNED)
    return "signed";
  if (type == KW_UNSIGNED)
    return "unsigned";
  if (type == KW_STRUCT)
    return "struct";
  if (type == KW_UNION)
    return "union";
  if (type == KW_ENUM)
    return "enum";
  if (type == KW_TYPEDEF)
    return "typedef";
  if (type == KW_SIZEOF)
    return "sizeof";
  if (type == KW_EXTERN)
    return "extern";
  if (type == KW_VOLATILE)
    return "volatile";
  if (type == KW_CONST)
    return "const";
  if (type == KW_AUTO)
    return "auto";
  if (type == KW_STATIC)
    return "static";
  if (type == KW_REGISTER)
    return "register";

  return "TK_UNKNOWN";
}


int
try_get_keyword(char *s)
{
  if (!scmp(s, "if"))
    return KW_IF;
  if (!scmp(s, "else"))
    return KW_ELSE;
  if (!scmp(s, "for"))
    return KW_FOR;
  if (!scmp(s, "while"))
    return KW_WHILE;
  if (!scmp(s, "do"))
    return KW_DO;
  if (!scmp(s, "switch"))
    return KW_SWITCH;
  if (!scmp(s, "case"))
    return KW_CASE;
  if (!scmp(s, "default"))
    return KW_DEFAULT;
  if (!scmp(s, "return"))
    return KW_RETURN;
  if (!scmp(s, "goto"))
    return KW_GOTO;
  if (!scmp(s, "continue"))
    return KW_CONTINUE;
  if (!scmp(s, "break"))
    return KW_BREAK;
  if (!scmp(s, "void"))
    return KW_VIOD;
  if (!scmp(s, "char"))
    return KW_CHAR;
  if (!scmp(s, "int"))
    return KW_INT;
  if (!scmp(s, "float"))
    return KW_FLOAT;
  if (!scmp(s, "double"))
    return KW_DOUBLE;
  if (!scmp(s, "short"))
    return KW_SHORT;
  if (!scmp(s, "long"))
    return KW_LONG;
  if (!scmp(s, "signed"))
    return KW_SIGNED;
  if (!scmp(s, "unsigned"))
    return KW_UNSIGNED;
  if (!scmp(s, "struct"))
    return KW_STRUCT;
  if (!scmp(s, "union"))
    return KW_UNION;
  if (!scmp(s, "enum"))
    return KW_ENUM;
  if (!scmp(s, "typedef"))
    return KW_TYPEDEF;
  if (!scmp(s, "sizeof"))
    return KW_SIZEOF;
  if (!scmp(s, "extern"))
    return KW_EXTERN;
  if (!scmp(s, "volatile"))
    return KW_VOLATILE;
  if (!scmp(s, "const"))
    return KW_CONST;
  if (!scmp(s, "auto"))
    return KW_AUTO;
  if (!scmp(s, "static"))
    return KW_STATIC;
  if (!scmp(s, "register"))
    return KW_REGISTER;
  return 0;
}

