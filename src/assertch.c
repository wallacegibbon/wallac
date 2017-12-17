#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "vars.h"


void
assert_not_eof(char ch)
{
  if (ch == EOF)
    exit_with_info("[%d][LEXER]Unexpected EOF\n", current_line);
}


void
assert_not_ch(char ch, char unexpected_ch)
{
  if (ch == unexpected_ch)
    exit_with_info("[%d][LEXER]Unexpected ch: %c\n", current_line, ch);
}


void
assert_ch(char ch, char expected_ch)
{
  if (ch == expected_ch)
    exit_with_info("[%d][LEXER]Expected %c, not %c\n", expected_ch, ch);
}


void
assert_octal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expected octal\n", current_line);
}


void
assert_decimal(char ch)
{
  if (!check_decimal(ch))
    exit_with_info("[%d][LEXER]Expected decimal\n", current_line);
}


void
assert_hex(char ch)
{
  if (!check_hex(ch))
    exit_with_info("[%d][LEXER]Expected hex\n", current_line);
}


void
assert_ident(char ch)
{
  if (!check_identity(ch))
    exit_with_info("[%d][LEXER]Expected identity\n", current_line);
}


