#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "limits.h"
#include "vars.h"
#include "token.h"
#include "lexer.h"


int current_line = 1;
int current_ch = 0;

char buff_tmp[MAX_CSTR_LENGTH];


int
next_char()
{
  current_ch = fgetc(fp_in);
  if (current_ch == EOF && ferror(fp_in))
    exit_with_info("Failed reading input file, %d\n", errno);

  if (current_ch == '\n')
    current_line++;

  return current_ch;
}


int
tokenize()
{
  initialize_token_list();

  next_char();
  while (get_token());

  return 0;
}


int
get_token()
{
  while (check_space(current_ch))
    next_char();

  if (current_ch == EOF)
    return 0;

  if (check_identifier_start(current_ch))
    return get_identifier();

  if (check_decimal(current_ch))
    return get_integer();

  if (current_ch == '"')
    return get_string();

  if (current_ch == '\'')
    return get_character();

  if (current_ch == '+')
    return get_plus_dplus();

  if (current_ch == '-')
    return get_minus_dminus_pointsto();

  if (current_ch == '.')
    return get_dot_ellipsis();

  if (current_ch == '&')
    return get_and_dand();

  if (current_ch == '|')
    return get_or_dor();

  if (current_ch == '=')
    return get_assign_eq();

  if (current_ch == '>')
    return get_gt_geq();

  if (current_ch == '<')
    return get_lt_leq();

  if (current_ch == '!')
    return get_exclamation_neq();

  if (current_ch == '/')
    return get_divide_or_jump_comments();

  if (current_ch == ';')
    return get_single(TK_SEMICOLON);

  if (current_ch == ':')
    return get_single(TK_COLON);

  if (current_ch == ',')
    return get_single(TK_COMMA);

  if (current_ch == '?')
    return get_single(TK_QUESTION);

  if (current_ch == '^')
    return get_single(TK_CARET);

  if (current_ch == '~')
    return get_single(TK_TILDE);

  if (current_ch == '*')
    return get_single(TK_ASTERISK);

  if (current_ch == '%')
    return get_single(TK_MOD);

  if (current_ch == '{')
    return get_single(TK_BEGIN);

  if (current_ch == '}')
    return get_single(TK_END);

  if (current_ch == '[')
    return get_single(TK_OPENBR);

  if (current_ch == ']')
    return get_single(TK_CLOSEBR);

  if (current_ch == '(')
    return get_single(TK_OPENPA);

  if (current_ch == ')')
    return get_single(TK_CLOSEPA);

  exit_with_info("[%d][LEXER]Unknown char: %02X\n",
      current_line, current_ch);

  return 1;
}


int
get_integer()
{
  char ch = current_ch;

  if (ch != '0')
    return get_decimal();

  ch = next_char();

  if (check_octal(ch))
    return get_octal();

  if (!check_decimal(ch))
    return get_zero();

  assert_ch(UPPER(ch), 'X');
  next_char();

  return get_hex();
}


int
get_zero()
{
  join_token(TK_CINT, (void *) 0);
  return 1;
}


int
get_octal()
{
  long i = 0;

  get_numstr(check_octal);
  i = get_numval(8, cnv_digit);

  join_token(TK_CINT, (void *)i);
  return 1;
}


int
get_decimal()
{
  long i = 0;

  get_numstr(check_decimal);
  i = get_numval(10, cnv_digit);

  join_token(TK_CINT, (void *)i);
  return 1;
}


int
get_hex()
{
  long i = 0;

  get_numstr(check_hex);
  i = get_numval(16, cnv_hexdigit);

  join_token(TK_CINT, (void *)i);
  return 1;
}


void
get_numstr(int chkfn(char))
{
  char *buffer = buff_tmp;
  int cnt = 1;

  *buffer++ = current_ch;

  while (cnt++ < MAX_INT_LENGTH-1 && chkfn(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_INT_LENGTH)
    exit_with_info("[%d][LEXER]Number too long\n", current_line);

  *buffer = '\0';
}


int
get_numval(int base, int cnvfn(char))
{
  long i = 0, j, k;
  char *buffer = buff_tmp;
  char *cmpstr =
    base == 8 ? MAX_OCTAL_STRING :
      base == 10 ? MAX_DECIMAL_STRING : MAX_HEX_STRING;

  j = strlen(buffer);
  k = strlen(cmpstr);
  if (j > k || (j == k && strcmp(buffer, cmpstr) > 0))
    return LONG_MAX;

  while (*buffer)
    i = i * base + cnvfn(*buffer++);

  return i;
}


int
cnv_hexdigit(char ch)
{
  if (ch >= 'a' && ch <= 'f')
    return ch - 'a' + 10;

  if (ch >= 'A' && ch <= 'F')
    return ch - 'A' + 10;

  return ch - '0';
}


int
cnv_digit(char ch)
{
  return ch - '0';
}


int
get_identifier()
{
  char *buffer = buff_tmp;
  char *p;
  int cnt = 1;

  *buffer++ = current_ch;

  while (cnt++ < MAX_IDENT_LENGTH-1 && check_identifier(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_IDENT_LENGTH)
    exit_with_info("[%d][LEXER]Identifier too long\n", current_line);

  *buffer = '\0';

  p = malloc(strlen(buff_tmp) + 1);
  strcpy(p, buff_tmp);

  join_token(TK_IDENT, p);
  return 1;
}


int
get_string()
{
  char *buffer = buff_tmp;
  char *p;
  int cnt = 0;
  char ch;

  while (cnt++ < MAX_CSTR_LENGTH-1 && get_strchar(&ch))
    *buffer++ = ch;

  if (cnt == MAX_CSTR_LENGTH)
    exit_with_info("[%d][LEXER]String too long\n", current_line);

  *buffer = '\0';

  p = malloc(strlen(buff_tmp) + 1);
  strcpy(p, buff_tmp);

  join_token(TK_CSTR, p);

  next_char();
  return 1;
}


int
get_strchar(char *ch)
{
  char c = next_char();
  assert_not_eof(c);
  assert_not_ch(c, '\n');
  if (c == '"')
    return 0;

  if (c == '\\')
    c = get_escape_seq();

  *ch = c;
  return 1;
}


int
get_character()
{
  char ch = next_char();
  assert_not_eof(ch);
  assert_not_ch(ch, '\'');

  if (ch == '\\')
    ch = get_escape_seq();

  assert_ch(next_char(), '\'');

  join_token(TK_CCHAR, (void *) ch);

  next_char();
  return 1;
}


int
get_escape_seq()
{
  char ch = next_char();
  assert_not_eof(ch);
  switch (ch)
  {
  case 'x':
    return get_hexnum() * 16 + get_hexnum();
  case 'a':
    return 7;
  case 'b':
    return 8;
  case 't':
    return 9;
  case 'n':
    return 10;
  case 'v':
    return 11;
  case 'f':
    return 12;
  case 'r':
    return 13;
  case '0':
    return 0;
  default:
    return ch;
  }
}


int
get_hexnum()
{
  char ch = next_char();
  assert_not_eof(ch);
  assert_hex(ch);
  return cnv_hexdigit(ch);
}


int
get_plus_dplus()
{
  next_char();
  if (current_ch == '+')
    return get_single(TK_DPLUS);

  join_token(TK_PLUS, NULL);
  return 1;
}


int
get_minus_dminus_pointsto()
{
  next_char();
  if (current_ch == '-')
    return get_single(TK_DMINUS);

  if (current_ch == '>')
    return get_single(TK_POINTSTO);

  join_token(TK_MINUS, NULL);
  return 1;
}


int
get_ellipsis()
{
  if (next_char() != '.')
    exit_with_info("[%d][LEXER]Unsupported element \"..\"\n", current_line);

  join_token(TK_ELLIPSIS, NULL);
  next_char();
  return 1;
}


int
get_dot_ellipsis()
{
  if (next_char() == '.')
    return get_ellipsis();

  join_token(TK_DOT, NULL);
  return 1;
}


int
get_and_dand()
{
  if (next_char() == '&')
    return get_single(TK_DAND);

  join_token(TK_AND, NULL);
  return 1;
}


int
get_or_dor()
{
  if (next_char() == '|')
    return get_single(TK_DOR);

  join_token(TK_DOR, NULL);
  return 1;
}


int
get_assign_eq()
{
  if (next_char() == '=')
    return get_single(TK_EQ);

  join_token(TK_ASSIGN, NULL);
  return 1;
}


int
get_gt_geq()
{
  if (next_char() == '=')
    return get_single(TK_GEQ);

  join_token(TK_GT, NULL);
  return 1;
}


int
get_lt_leq()
{
  if (next_char() == '=')
    return get_single(TK_LEQ);

  join_token(TK_LT, NULL);
  return 1;
}


int
get_exclamation_neq()
{
  if (next_char() == '=')
    return get_single(TK_NEQ);

  join_token(TK_EXCLAMATION, NULL);
  return 1;
}


int
get_single(short type)
{
  join_token(type, NULL);
  next_char();
  return 1;
}


int
jump_multi_comments()
{
  next_char();
  while (next_char() != '*');

  if (next_char() != '/')
    jump_multi_comments();
  else
    return 1;
}


int
jump_line_comments()
{
  while (next_char() != '\n');

  return 1;
}


int
get_divide_or_jump_comments()
{
  char ch = next_char();
  if (ch == '*')
    return jump_multi_comments();

  if (ch == '/')
    return jump_line_comments();

  join_token(TK_DIVIDE, NULL);
  return 1;
}


