#ifndef __LEXER_H__
#define __LEXER_H__


#include "token.h"

int
next_char();

int
tokenize();

int
get_token();

int
get_integer();

int
get_zero(int line);

int
get_octal(int line);

int
get_decimal(int line);

int
get_hex(int line);

int
get_character();

int
get_string();

int
get_identifier();

void
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
get_plus_dplus();

int
get_minus_dminus_pointsto();

int
get_dot_ellipsis();

int
get_and_dand();

int
get_or_dor();

int
get_assign_eq();

int
get_gt_geq();

int
get_lt_leq();

int
get_exclamation_neq();

int
get_single(int line, int type);

int
get_divide_or_jump_comments();


#endif
