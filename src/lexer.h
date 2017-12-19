#ifndef __LEXER_H__
#define __LEXER_H__


#include "token.h"

int
tokenize();

int
get_token();

struct token *
slide_tk(struct token **p);

void
join_token(short type, void *p);

void
print_token(struct token *t);

int
get_integer();

int
get_octal();

int
get_decimal();

int
get_hex();

int
get_character();

int
get_string();

int
get_identifier();

int
get_numstr(int chkfn(char));

int
get_numval(int base, int cnvfn(char));

int
cnv_digit(char ch);

int
cnv_hexdigit(char ch);

int
get_strchar(char *ch);

int
get_escape_seq();

int
get_hexnum();

int
next_char();


#endif
