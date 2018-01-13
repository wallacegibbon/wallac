#ifndef __LEXER_H__
#define __LEXER_H__


#include <stdio.h>
#include "hashtbl.h"

struct lex { int ch, pch, line, cursor; char *fname; FILE *fp; char *str;
  char *buff; struct hashtbl *mtbl; struct token *tk_s, *tk_c; };


struct token *
tokenize_base(struct lex *lx);

struct token *
tokenize();

#endif
