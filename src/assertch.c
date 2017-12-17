#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "vars.h"


int
assert_not_eof(char ch)
{
  if (ch == EOF)
    exit_with_info("[%d][LEXER]Unexpected EOF\n", current_line);
  return ch;
}


int
assert_not_ch(char ch, char unexpected_ch)
{
  if (ch == unexpected_ch)
    exit_with_info("[%d][LEXER]Unexpected ch: \"%c\"\n", current_line, ch);
  return ch;
}


int
assert_ch(char ch, char expected_ch)
{
  if (ch != expected_ch)
    exit_with_info("[%d][LEXER]Expect \"%c\"(not \"%c\")\n", expected_ch, ch);
  return ch;
}


int
assert_octal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expect octal\n", current_line);
  return ch;
}


int
assert_decimal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expect decimal\n", current_line);
  return ch;
}


int
assert_hex(char ch)
{
  if (!check_hex(ch))
    exit_with_info("[%d][LEXER]Expect hex\n", current_line);
  return ch;
}


int
assert_ident(char ch)
{
  if (!check_identity(ch))
    exit_with_info("[%d][LEXER]Expect identity\n", current_line);
  return ch;
}


