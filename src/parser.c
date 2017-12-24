#include "lexer.h"
#include "token.h"

/*

if (expression) statement1
if (expression) statement1 else statement2

for ([expr1]; [expr2]; [expr3]) statement
while (expression) statement

return [expression];
goto identifier;

extern data-definition;
extern function-prototype;

volatile data-definition;

*/


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


