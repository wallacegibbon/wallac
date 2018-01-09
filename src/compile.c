#include "token.h"
#include "lexer.h"
#include "vars.h"


int
compile()
{
  tokenize();

  if (verbose)
    print_token_list();

  return 0;
}

