#include "lexer.h"


int
compile()
{
  tokenize();
  print_token_list();

  return 0;
}

