#ifndef __LEXER_H__
#define __LEXER_H__


#include "hashtbl.h"

struct lex { int ch, pch, line, cursor; int type; char *fname; int fd; int eof;
  char *buff, *input; struct hashtbl *mtbl; struct token *tk_s, *tk_c; };


struct token *
tokenize_base(struct lex *lx);

struct token *
tokenize();

#endif
