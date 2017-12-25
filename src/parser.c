#include "lexer.h"
#include "token.h"


#define STAT_IF 1
#define STAT_IF_ELSE 2
#define STAT_FOR 3
#define STAT_WHILE 4
#define STAT_EXPRESSION 5
#define STAT_RETURN 6
#define STAT_LABEL 8
#define STAT_GOTO 7

#define EXPR_ADD 1
#define EXPR_MINUS 2
#define EXPR_MUL 3
#define EXPR_DIV 4
#define EXPR_MOD 5
#define EXPR_AND 6
#define EXPR_OR 7
#define EXPR_XOR 8
#define EXPR_REV 9 //~
#define EXPR_NOT 10
#define EXPR_DAND 11
#define EXPR_DOR 12
#define EXPR_ASSIGN 13
#define EXPR_GT 14
#define EXPR_GEQ 15
#define EXPR_LT 16
#define EXPR_LEQ 17
#define EXPR_EQ 18
#define EXPR_NEQ 19
#define EXPR_POINTER 20 //*x
#define EXPR_ADDRESS 21 //&x
#define EXPR_CALL 22 //x()
#define EXPR_ARR 23 //x[]
#define EXPR_DOT 24 //x.a
#define EXPR_POINTSTO 25 //x->a
#define EXPR_VAR_GLOBAL 26
#define EXPR_VAR_LOCAL 27
#define EXPR_VAR_ARG 28
#define EXPR_NUMBER 29
#define EXPR_STRING 30
#define EXPR_CHAR 31


struct expr { struct expr *next; int type; void *ops; };

struct stat { struct stat *next; int type; void *v; };
struct stat_if { struct expr *cond; struct stat *t; };
struct stat_if_else { struct expr *cond; struct stat *t, *f; };
struct stat_for { struct expr *e1, *e2, *e3; struct stat *s; };
struct stat_while { struct expr *cond; struct stat *s; };
struct stat_expr { struct expr *expr; };
struct stat_return { struct expr *expr; };
struct stat_goto { int lbidx; };

struct label { int stat_idx; char *name; };

struct fn { struct cvars *as, *vs; struct label *lbs; struct stat *sta; };



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


