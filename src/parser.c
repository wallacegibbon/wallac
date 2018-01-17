#include <stdlib.h>
#include "libc.h"
#include "misc.h"
#include "token.h"
#include "hashtbl.h"



struct parser { struct token *tk;
  struct hashtbl *sdefs, *gvars, *funcs; };



struct parser *
new_parser(struct token *tks)
{
  struct parser *p;
  p = malloc(sizeof(struct parser));
  if (!p)
    exit_with("Failed alloc memory for parser\n");

  p->funcs = new_hashtbl(40);
  p->gvars = new_hashtbl(20);
  p->sdefs = new_hashtbl(20);

  p->tk = tks;

  return p;
}


int
struct_def_or_var(struct parser *psr)
{
  pf("struct_def_or_var...\n");

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


