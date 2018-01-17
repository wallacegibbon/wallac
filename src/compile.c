#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "vars.h"


int
compile(char *filename)
{
  struct token *tks;

  tks = tokenize(filename);

  if (verbose)
    print_token_list(tks);

  parse(tks);

  return 0;
}

