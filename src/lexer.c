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


#define UPPER(ch) ((ch) & ~(1 << 5))

struct node *start_tk = NULL, *current_tk = NULL;
int current_line = 1;
int current_ch = 0;

char buff_tmp[MAX_CSTR_LENGTH];



int get_token();

int get_integer();
int get_octal();
int get_decimal();
int get_hex();
int get_numstr(int chkfn(char));
int get_numval(int base, int cnvfn(char));
int cnv_digit(char ch);
int cnv_hexdigit(char ch);
int get_character();
int get_string();
int get_strchar(char *ch);
int get_identifier();
int get_escape_seq();
int get_hexnum();
int next_char();


int
tokenize()
{
  start_tk = malloc(sizeof(struct node));
  if (!start_tk)
    exit_with_info("Failed alloc memory for start_tk\n");

  current_tk = start_tk;

  next_char();
  while (get_token() != TK_EOF);

  return 0;
}


int
get_token()
{
  while (check_space(current_ch))
    next_char();

  if (current_ch == EOF)
    return TK_EOF;

  if (check_identifier_start(current_ch))
    return get_identifier();

  if (check_decimal(current_ch))
    return get_integer();

  if (current_ch == '"')
    return get_string();

  if (current_ch == '\'')
    return get_character();

  exit_with_info("**Unknown char: %02X\n", current_ch);

  return 0;
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

  assert_ch(UPPER(ch), 'X');
  next_char();

  return get_hex();
}


int
get_octal()
{
  long i = 0;

  get_numstr(check_octal);
  i = get_numval(8, cnv_digit);

  printf("int: %d(0o%o)\n", i, i);
  return TK_CINT;
}


int
get_decimal()
{
  long i = 0;

  get_numstr(check_decimal);
  i = get_numval(10, cnv_digit);

  printf("int: %d\n", i);
  return TK_CINT;
}


int
get_hex()
{
  long i = 0;

  get_numstr(check_hex);
  i = get_numval(16, cnv_hexdigit);

  printf("int: %d(0X%X)\n", i, i);
  return TK_CINT;
}


int
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

  printf("int str: %s\n", buff_tmp);

  return 0;
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
  int cnt = 1;

  *buffer++ = current_ch;

  while (cnt++ < MAX_IDENT_LENGTH-1 && check_identifier(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_IDENT_LENGTH)
    exit_with_info("[%d][LEXER]Identifier too long\n", current_line);

  *buffer = '\0';

  printf("identifier: %s\n", buff_tmp);

  return TK_IDENT;
}


int
get_string()
{
  char *buffer = buff_tmp;
  int cnt = 0;
  char ch;

  while (cnt++ < MAX_CSTR_LENGTH-1 && get_strchar(&ch))
    *buffer++ = ch;

  if (cnt == MAX_CSTR_LENGTH)
    exit_with_info("[%d][LEXER]String too long\n", current_line);

  *buffer = '\0';

  printf("string: %s\n", buff_tmp);

  next_char();
  return TK_CSTR;
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

  printf("character: %02X\n", ch);

  next_char();
  return TK_CCHAR;
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
next_char()
{
  current_ch = fgetc(fp_in);
  if (current_ch == EOF && ferror(fp_in))
    exit_with_info("Failed reading input file, %d\n", errno);

  if (current_ch == '\n')
    current_line++;

  return current_ch;
}


