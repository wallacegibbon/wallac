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


struct token *
nexttoken(struct parser *psr)
{
  struct token *tk;
  if (!psr->tk)
    return NULL;

  psr->tk = psr->tk->next;
  return psr->tk;
}


int
check_unsupported_keyword(int type)
{
  return type == KW_VOLATILE || type == KW_CONST || type == KW_AUTO ||
    type == KW_STATIC || type == KW_REGISTER ||
    type == KW_FLOAT || type == KW_DOUBLE ||
    type == KW_SWITCH || type == KW_CASE ||
    type == KW_BREAK ||
    type == KW_CONTINUE ||
    type == KW_GOTO ||
    type == KW_TYPEDEF;
}


int
filter_unsupported_tk(struct token *tk)
{
  if (tk->type == TK_OPENBR || tk->type == TK_CLOSEBR)
    exit_with("%s:%d:[PARSER]Original C array is not supported\n",
        tk->fname, tk->line);

  if (check_unsupported_keyword(tk->type))
    exit_with("%s:%d:[PARSER]Unsupported keyword: %s\n",
        tk->fname, tk->line, token_type_str(tk->type));

  return 1;
}


int
assert_notlast_tk(struct parser *psr)
{
  if (!psr->tk)
    exit_with("UNKNOWN:UNKNOWN:[PARSER]Unexpected end of file\n");

  if (!psr->tk->next)
    exit_with("%s:%d:[PARSER]Unexpected end of file\n",
        psr->tk->fname, psr->tk->line);
}


int
analysis_type(struct parser *psr)
{
  struct token *tk;
  struct ctype *ct;

  //ct = new_ctype();
 // new_ctype(int type, int structidx, struct ctype *fnret,
 //   struct linktbl *fnparams)
  tk = psr->tk;
}


int
struct_def_or_var(struct parser *psr)
{
  struct token *tk;
  char *sname;

  nexttoken(psr);
  assert_notlast_tk(psr);

  if (psr->tk->type != TK_IDENT)
    exit_with("%s:%d:[PARSER]Struct name expected\n",
        psr->tk->fname, psr->tk->line);

  sname = (char *) psr->tk->value;

  nexttoken(psr);
  assert_notlast_tk(psr);

  //if (tk->type == TK_BEGIN)
  //  return struct_def(psr, sname);

  //if (tk->type == TK_IDENT)
  //  return struct_var_decl(psr, sname);

  exit_with("%s:%d:[PARSER]Invalid struct syntax\n",
      tk->fname, tk->line);

  return 1;
}


int
extern_var_decl(struct parser *psr)
{
  pf("extern_var_decl...\n");

  return 1;
}


int
decl_or_def(struct parser *psr)
{
  struct token *tk;

  tk = psr->tk;
  if (tk->type == KW_STRUCT)
    return struct_def_or_var(psr);
  if (tk->type == KW_EXTERN)
    return extern_var_decl(psr);

  exit_with("%s:%d:[PARSER]Unexpected token %d\n",
      tk->fname, tk->line, tk->type);
}


int
parse(struct token *tks)
{
  struct parser *psr;

  psr = new_parser(tks);

  decl_or_def(psr);

  return 1;
}


