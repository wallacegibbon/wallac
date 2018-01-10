#ifndef __ASSERT_H__
#define __ASSERT_H__


#include "lexer.h"


int
assert_not_eof(struct lex *lx, char ch);

int
assert_not_ch(struct lex *lx, char ch, char unexpected_ch);

int
assert_ch(struct lex *lx, char ch, char expected_ch);

int
assert_octal(struct lex *lx, char ch);

int
assert_decimal(struct lex *lx, char ch);

int
assert_hex(struct lex *lx, char ch);

int
assert_ident(struct lex *lx, char ch);


#endif
