#include "lexer.h"
#include "token.h"


int
compile()
{
  tokenize();
  print_token_list();

  return 0;
}

