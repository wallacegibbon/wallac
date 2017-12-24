#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "misc.h"


struct token *start_tk = NULL, *current_tk = NULL;

struct kw_pair kw_map[32] =
{
  { KW_IF, "if" },
  { KW_ELSE, "else" },
  { KW_FOR, "for" },
  { KW_WHILE, "while" },
  { KW_DO, "do" },
  { KW_SWITCH, "switch" },
  { KW_CASE, "case" },
  { KW_DEFAULT, "default" },
  { KW_RETURN, "return" },
  { KW_GOTO, "goto" },
  { KW_CONTINUE, "continue" },
  { KW_BREAK, "break" },
  { KW_VIOD, "void" },
  { KW_CHAR, "char" },
  { KW_INT, "int" },
  { KW_FLOAT, "float" },
  { KW_DOUBLE, "double" },
  { KW_SHORT, "short" },
  { KW_LONG, "long" },
  { KW_SIGNED, "signed" },
  { KW_UNSIGNED, "unsigned" },
  { KW_STRUCT, "struct" },
  { KW_UNION, "union" },
  { KW_ENUM, "enum" },
  { KW_TYPEDEF, "typedef" },
  { KW_SIZEOF, "sizeof" },
  { KW_EXTERN, "extern" },
  { KW_VOLATILE, "volatile" },
  { KW_CONST, "const" },
  { KW_AUTO, "auto" },
  { KW_STATIC, "static" },
  { KW_REGISTER, "register" }
};



void
initialize_token_list()
{
  start_tk = malloc(sizeof(struct token));
  if (!start_tk)
    exit_with_info("Failed alloc memory for start_tk\n");

  start_tk->type = 0;
  start_tk->prev = NULL;
  start_tk->next = NULL;
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
  t->prev = current_tk;
  t->next = NULL;
  current_tk->next = t;
  current_tk = t;
}


void
print_token_list()
{
  struct token *p = start_tk->next;

  for (; p; p = p->next)
    print_token(p);
}


void
print_token_value(void *raw)
{
  int v = (int) raw;
  printf("0x%x, 0o%o, %d\n", v, v, v);
}


void
print_token(struct token *t)
{
  printf("(%d)[%s]", t->line, token_type_str(t->type));
  if (t->type != TK_CSTR && t->type != TK_IDENT)
    print_token_value(t->value);
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
  if (type == KW_IF)
    return "KW_IF";
  if (type == KW_ELSE)
    return "KW_ELSE";
  if (type == KW_WHILE)
    return "KW_WHILE";
  if (type == KW_SWITCH)
    return "KW_SWITCH";
  if (type == KW_CASE)
    return "KW_CASE";
  if (type == KW_DEFAULT)
    return "KW_DEFAULT";
  if (type == KW_FOR)
    return "KW_FOR";
  if (type == KW_DO)
    return "KW_DO";
  if (type == KW_RETURN)
    return "KW_RETURN";
  if (type == KW_GOTO)
    return "KW_GOTO";
  if (type == KW_CONTINUE)
    return "KW_CONTINUE";
  if (type == KW_BREAK)
    return "KW_BREAK";
  if (type == KW_VIOD)
    return "KW_VIOD";
  if (type == KW_CHAR)
    return "KW_CHAR";
  if (type == KW_INT)
    return "KW_INT";
  if (type == KW_FLOAT)
    return "KW_FLOAT";
  if (type == KW_DOUBLE)
    return "KW_DOUBLE";
  if (type == KW_SHORT)
    return "KW_SHORT";
  if (type == KW_LONG)
    return "KW_LONG";
  if (type == KW_SIGNED)
    return "KW_SIGNED";
  if (type == KW_UNSIGNED)
    return "KW_UNSIGNED";
  if (type == KW_STRUCT)
    return "KW_STRUCT";
  if (type == KW_UNION)
    return "KW_UNION";
  if (type == KW_ENUM)
    return "KW_ENUM";
  if (type == KW_TYPEDEF)
    return "KW_TYPEDEF";
  if (type == KW_SIZEOF)
    return "KW_SIZEOF";
  if (type == KW_EXTERN)
    return "KW_EXTERN";
  if (type == KW_VOLATILE)
    return "KW_VOLATILE";
  if (type == KW_CONST)
    return "KW_CONST";
  if (type == KW_AUTO)
    return "KW_AUTO";
  if (type == KW_STATIC)
    return "KW_STATIC";
  if (type == KW_REGISTER)
    return "KW_REGISTER";

  return "TK_UNKNOWN";
}


