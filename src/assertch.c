#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "vars.h"


int
assert_not_eof(struct lex *lx, char ch)
{
  if (ch == EOF)
    exit_with("%f:%d:[LEXER]Unexpected EOF\n",
        lx->fname, lx->line);

  return ch;
}


int
assert_not_ch(struct lex *lx, char ch, char unexpected_ch)
{
  if (ch == unexpected_ch)
    exit_with("%s:%d:[LEXER]Unexpected char (0x%x)\n",
        lx->fname, lx->line, ch);

  return ch;
}


int
assert_ch(struct lex *lx, char ch, char expected_ch)
{
  if (ch != expected_ch)
    exit_with("%s:%d:[LEXER]Expect char (0x%x), not (0x%x)\n",
        lx->fname, lx->line, expected_ch, ch);

  return ch;
}


int
assert_octal(struct lex *lx, char ch)
{
  if (!check_decimal(ch))
    exit_with("%s:%d:[LEXER]Expect octal char\n",
        lx->fname, lx->line);

  return ch;
}


int
assert_decimal(struct lex *lx, char ch)
{
  if (!check_decimal(ch))
    exit_with("%s:%d:[LEXER]Expect decimal char\n",
        lx->fname, lx->line);

  return ch;
}


int
assert_hex(struct lex *lx, char ch)
{
  if (!check_hex(ch))
    exit_with("%s:%d:[LEXER]Expect hex char\n",
        lx->fname, lx->line);

  return ch;
}


int
assert_ident(struct lex *lx, char ch)
{
  if (!check_ident(ch))
    exit_with("%s:%d:[LEXER]Expect identifier\n",
        lx->fname, lx->line);

  return ch;
}


