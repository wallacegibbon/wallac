#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "vars.h"


int
assert_not_eof(char ch)
{
  if (ch == EOF)
    exit_with_info("%f:%d:[LEXER]Unexpected EOF\n",
        filename, current_line);
  return ch;
}


int
assert_not_ch(char ch, char unexpected_ch)
{
  if (ch == unexpected_ch)
    exit_with_info("%s:%d:[LEXER]Unexpected char [0x%x]\n",
        filename, current_line, ch);
  return ch;
}


int
assert_ch(char ch, char expected_ch)
{
  if (ch != expected_ch)
    exit_with_info("%s:%d:[LEXER]Expect char [0x%x](not [0x%x])\n",
        filename, current_line, expected_ch, ch);
  return ch;
}


int
assert_octal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("%s:%d:[LEXER]Expect octal char\n",
        filename, current_line);
  return ch;
}


int
assert_decimal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("%s:%d:[LEXER]Expect decimal char\n",
        filename, current_line);
  return ch;
}


int
assert_hex(char ch)
{
  if (!check_hex(ch))
    exit_with_info("%s:%d:[LEXER]Expect hex char\n",
        filename, current_line);
  return ch;
}


int
assert_ident(char ch)
{
  if (!check_ident(ch))
    exit_with_info("%s:%d:[LEXER]Expect identifier\n",
        filename, current_line);
  return ch;
}


