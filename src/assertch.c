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
    exit_with_info("[%d][LEXER]Unexpected char [%02X]\n", current_line, ch);
  return ch;
}


int
assert_ch(char ch, char expected_ch)
{
  if (ch != expected_ch)
    exit_with_info("[%d][LEXER]Expect char [%02X](not [%02X])\n",
                   current_line, expected_ch, ch);
  return ch;
}


int
assert_octal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expect octal char\n", current_line);
  return ch;
}


int
assert_decimal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expect decimal char\n", current_line);
  return ch;
}


int
assert_hex(char ch)
{
  if (!check_hex(ch))
    exit_with_info("[%d][LEXER]Expect hex char\n", current_line);
  return ch;
}


int
assert_ident(char ch)
{
  if (!check_identifier(ch))
    exit_with_info("[%d][LEXER]Expect identifier\n", current_line);
  return ch;
}


