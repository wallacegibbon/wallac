#include "lexer.h"
#include "token.h"


int
parse()
{
  tokenize();
  print_token_list();

  rec_tokens(start_tk->r);
}


int
rec_tokens(struct token *t)
{
  if (t->r)
    rec_tokens(t->r);
}


