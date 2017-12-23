#include "lexer.h"
#include "token.h"

int
rec_tokens(struct token *t);

int
parse()
{
  tokenize();
  print_token_list();

  rec_tokens(start_tk->next);

  return 0;
}


int
rec_tokens(struct token *t)
{
  if (t->next)
    return rec_tokens(t->next);

  return 0;
}


