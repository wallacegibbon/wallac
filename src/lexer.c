#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "limit.h"
#include "vars.h"
#include "token.h"


#define UPPER(ch) (ch & ~(1 << 5))

struct node *start_tk = NULL, *current_tk = NULL;
int current_line = 1;
int current_ch = 0;

char cstr_buff[MAX_CSTR_LENGTH];
char cint_buff[MAX_INT_LENGTH];
char cident_buff[MAX_IDENT_LENGTH];



int get_token();

int get_integer();
int get_character();
int get_string();
int get_char();
int get_identity();
int get_escape_seq();
int get_hex_num();
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

  printf("Current ch:%2X\n", current_ch);

  if (check_identity_start(current_ch))
    return get_identity();

  if (check_decimal(current_ch))
    return get_integer();

  if (current_ch == '"')
    return get_string();

  if (current_ch == '\'')
    return get_character();

  if (current_ch == EOF)
    return TK_EOF;

  exit_with_info("Unknown ch: %2X\n", current_ch);

  return 0;
}


int
get_integer()
{
  char ch = current_ch;

  if (ch != '0')
    return get_integer_sub(check_decimal);

  ch = next_char();

  if (check_octal(ch))
    return get_integer_sub(check_octal);

  assert_ch(UPPER(ch), 'X');

  next_char();
  return get_integer_sub(check_hex);
}


int
get_integer_sub(int fn(char))
{
  char *buffer = cint_buff;
  int cnt = 1;

  *buffer++ = current_ch;

  while (cnt++ < MAX_INT_LENGTH-1 && fn(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_INT_LENGTH-1)
    exit_with_info("[%d][LEXER]Number too long\n", current_line);

  *buffer = '\0';

  printf("int: %s\n", cint_buff);

  return 0;
}


int
get_identity()
{
  char *buffer = cident_buff;
  int cnt = 1;

  *buffer++ = current_ch;

  while (cnt++ < MAX_IDENT_LENGTH-1 && check_identity(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_IDENT_LENGTH-1)
    exit_with_info("[%d][LEXER]Identity too long\n", current_line);

  *buffer = '\0';

  printf("identity: %s\n", cident_buff);

  return 0;
}


int
get_string()
{
  char *buffer = cstr_buff;
  int cnt = 0;
  char ch = next_char();

  while (cnt++ < MAX_CSTR_LENGTH-1 && ch != '"')
    *buffer++ = get_char();

  if (cnt == MAX_CSTR_LENGTH-1)
    exit_with_info("[%d][LEXER]String too long\n", current_line);

  *buffer = '\0';

  printf("string: %s\n", cstr_buff);

  return 0;
}


int
get_character()
{
  char ch = get_char();
  assert_ch(next_char(), '\'');

  printf("character: %c(%2X)\n", ch, ch);

  return ch;
}


int
get_char()
{
  char ch = next_char();
  assert_not_eof(ch);
  assert_not_ch(ch, '\'');

  if (ch == '\\')
    ch = get_escape_seq();

  return ch;
}


int
get_escape_seq()
{
  char ch = next_char();
  assert_not_eof(ch);

  switch (ch)
  {
  case 'x':
    return get_hex_num() * 16 + get_hex_num();
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
get_hex_num()
{
  char ch = next_char();
  assert_not_eof(ch);
  assert_hex(ch);
  
  if (ch >= 'a' && ch <= 'f')
    return ch - 'a';

  if (ch >= 'A' && ch <= 'F')
    return ch - 'A';

  return ch - '0';
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


