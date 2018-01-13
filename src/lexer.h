#ifndef __LEXER_H__
#define __LEXER_H__


#include <stdio.h>

struct lex { int ch, pch, line, cursor; char *fname; FILE *fp; char *str;
  struct token *tk_s, *tk_c; };


struct token *
tokenize();

#endif
