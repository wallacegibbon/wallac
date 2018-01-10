#ifndef __LEXER_H__
#define __LEXER_H__

struct lex { int (*nchar)(); int ch; int line; char *fname;
  struct token *tk_s, *tk_c; };

int
init_lexers();

struct token *
tokenize();

#endif
