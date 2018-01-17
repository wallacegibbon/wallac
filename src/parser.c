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
struct_var(struct parser *psr, char *sname)
{
  return 1;
}


int
struct_decl(struct parser *psr, char *sname)
{
  return 1;
}


int
struct_def_or_var(struct parser *psr)
{
  struct token *tk;
  char *sname;

  tk = psr->tk;

  tk = tk->next;
  if (tk->type != TK_IDENT)
    exit_with("%s:%d:[PARSER]Struct name expected\n",
        tk->fname, tk->line);

  sname = (char *) tk->value;

  tk = tk->next;
  psr->tk = tk;

  if (tk->type == TK_IDENT)
    return struct_var(psr, sname);

  if (tk->type == TK_BEGIN)
    return struct_decl(psr, sname);

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


