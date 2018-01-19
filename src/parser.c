#include <stdlib.h>
#include "libc.h"
#include "misc.h"
#include "linktbl.h"
#include "token.h"
#include "node.h"
#include "ctypes.h"



struct parser { struct token *tk; struct linktbl *sdefs, *gvars, *funcs; };



struct parser *
new_parser(struct token *tks)
{
  struct parser *p;

  p = malloc(sizeof(struct parser));
  if (!p)
    exit_with("Failed alloc memory for parser\n");

  p->funcs = new_linktbl();
  p->gvars = new_linktbl();
  p->sdefs = new_linktbl();

  p->tk = tks;

  return p;
}


int
check_unsupported_keyword(int type)
{
  return
    type == KW_VOLATILE ||
    type == KW_CONST ||
    type == KW_AUTO ||
    type == KW_STATIC ||
    type == KW_REGISTER ||
    type == KW_FLOAT ||
    type == KW_DOUBLE ||
    type == KW_SWITCH ||
    type == KW_CASE ||
    type == KW_CONTINUE ||
    type == KW_BREAK ||
    type == KW_GOTO ||
    type == KW_TYPEDEF;
}


int
filter_unsupported_tk(struct token *tk)
{
  if (tk->type == TK_OPENBR || tk->type == TK_CLOSEBR)
    exit_with("%s:%d:[PARSER]C array is not supported\n",
        tk->fname, tk->line);

  if (check_unsupported_keyword(tk->type))
    exit_with("%s:%d:[PARSER]Unsupported keyword: %s\n",
        tk->fname, tk->line, token_type_str(tk->type));

  return 1;
}


int
assert_ntk_notnull(struct parser *psr)
{
  if (!psr->tk->next)
    exit_with("%s:%d:[PARSER]Unexpected end of file\n",
        psr->tk->fname, psr->tk->line);
}


struct token *
nexttoken(struct parser *psr)
{
  struct token *tk;

  if (!psr->tk)
    return NULL;

  tk = psr->tk->next;

  if (tk)
    filter_unsupported_tk(tk);

  psr->tk = tk;
  return tk;
}


struct token *
nexttoken_notend(struct parser *psr)
{
  struct token *tk;

  tk = nexttoken(psr);
  assert_ntk_notnull(psr);

  return tk;
}


int
adjust_int_extra(struct parser *psr)
{
  struct token *tk;

  tk = psr->tk;
  if ((tk->type == KW_SHORT || tk->type == KW_LONG) &&
      tk->next->type == KW_INT)
    nexttoken_notend(psr);

  return 1;
}


int
get_int_width(struct parser *psr)
{
  struct token *tk;
  int type;

  tk = psr->tk;
  type = tk->type;

  adjust_int_extra(psr);
  nexttoken_notend(psr);

  if (type == KW_CHAR)
    return CT_CHAR;

  if (type == KW_SHORT)
    return CT_SHORT;

  if (type == KW_INT)
    return CT_INT;

  if (type == KW_LONG)
    return CT_LONG;

  exit_with("%s:%d:[PARSER]Invalid int variable declaration\n",
      tk->fname, tk->line);

  return 1;
}


int
get_unsigned_type(struct parser *psr)
{
  int type;

  nexttoken_notend(psr);
  type = get_int_width(psr);
  type |= 0x10;

  return type;
}


int
get_signed_type(struct parser *psr)
{
  int type;

  nexttoken_notend(psr);
  type = get_int_width(psr);

  return type;
}


int
get_void_type(struct parser *psr)
{
  nexttoken_notend(psr);
  return CT_VOID;
}


int
get_struct_type(struct parser *psr)
{
}


int
get_basic_type(struct parser *psr)
{
  struct token *tk;

  tk = psr->tk;

  if (tk->type == KW_STRUCT)
    return get_struct_type(psr);

  if (tk->type == KW_UNSIGNED)
    return get_unsigned_type(psr);

  if (tk->type == KW_SIGNED)
    return get_signed_type(psr);

  if (tk->type == KW_VIOD)
    return get_void_type(psr);

  return get_int_width(psr);
}


int
get_ctype(struct parser *psr, int is_global)
{
  struct token *tk;
  int is_extern, pdepth, type;
  char *name;

  if (!psr->tk)
    return 0;

  assert_ntk_notnull(psr);
  tk = psr->tk;
  if (tk->type == KW_EXTERN)
    is_extern = 1;
  else
    is_extern = 0;

  if (is_extern && !is_global)
    exit_with("%s:%d:[SEMANTIC]Only global variable can be extern\n",
        tk->fname, tk->line);

  if (is_extern)
    tk = nexttoken_notend(psr);

  type = get_basic_type(psr);

  pdepth = 0;
  for (; psr->tk->type == TK_ASTERISK; nexttoken_notend(psr))
    pdepth++;

  tk = psr->tk;

  if (tk->type != TK_IDENT)
    exit_with("%s:%d:[PARSER]Invalid variable declaration\n",
        tk->fname, tk->line);

  name = (char *) tk->value;

  pf("is_extern: %d, type: %x, pdepth: %d, name: %s\n",
      is_extern, type, pdepth, name);

  return 1;
}


int
parse(struct token *tks)
{
  struct parser *psr;

  psr = new_parser(tks);

  get_ctype(psr, 1);

  return 1;
}


