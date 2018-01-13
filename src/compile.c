#include "token.h"
#include "lexer.h"
#include "vars.h"


int
compile()
{
  struct token *tks;

  tks = tokenize();

  if (verbose)
    print_token_list(tks);

  return 0;
}

