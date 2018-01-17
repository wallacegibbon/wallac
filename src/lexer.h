#ifndef __LEXER_H__
#define __LEXER_H__


#include "hashtbl.h"

struct lex { int ch, pch, line, cursor; int type; char *buff, *input;
  char *fname; int fd; int eof;
  struct hashtbl *mtbl; struct token *tk_s, *tk_c; };


struct token *
tokenize_lx(struct lex *lx);

struct token *
tokenize(char *filename);

#endif
