#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "assertch.h"
#include "checkch.h"
#include "misc.h"
#include "limits.h"
#include "vars.h"
#include "token.h"
#include "lexer.h"
#include "hashtbl.h"



char buff_tmp[MAX_CSTR_LENGTH];
struct lex lxsrc;

struct hashtbl *macrotbl;



int
next_char()
{
  int ch;

  ch = fgetc(fp_in);
  if (ch == EOF && ferror(fp_in))
    exit_with_info("Failed reading input file, %d\n", errno);

  if (lxsrc.line >= INT_MAX)
    exit_with_info("File too long\n");

  if (ch == '\n')
    lxsrc.line++;

  lxsrc.pch = lxsrc.ch;
  lxsrc.ch = ch;

  return ch;
}


int
init_lexers()
{
  lxsrc.tk_s = new_token(0, NULL);
  lxsrc.tk_s->prev = NULL;
  lxsrc.tk_s->next = NULL;
  lxsrc.tk_c = lxsrc.tk_s;

  lxsrc.fname = filename;
  lxsrc.nchar = next_char;
  lxsrc.line = 1;
  lxsrc.ch = '\0';
  lxsrc.pch = '\0';

  macrotbl = new_hashtbl(20);
}


int
join_token(struct lex *lx, int line, int type, void *p)
{
  struct token *t;
  t = new_token(type, p);
  t->line = line;
  t->prev = lx->tk_c;
  t->next = NULL;
  lx->tk_c->next = t;
  lx->tk_c = t;

  return 1;
}


int
join_token_nchar(struct lex *lx, int line, int type)
{
  join_token(lx, line, type, NULL);
  lx->nchar();
  return 1;
}


int
get_ellipsis(struct lex *lx, int line)
{
  int ch;

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch != '.')
    exit_with_info("%s:%d:[LEXER]Unsupported element \"..\"\n",
        lx->fname, line);

  join_token(lx, line, TK_ELLIPSIS, NULL);
  lx->nchar();
  return 1;
}


int
get_dot_ellipsis(struct lex *lx)
{
  int line, ch; 

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '.')
    return get_ellipsis(lx, line);

  if (check_decimal(ch))
    exit_with_info("%s:%d:[LEXER]Float literal is not supported\n",
        lx->fname, line);

  return join_token(lx, line, TK_DOT, NULL);
}


int
get_minus_dminus_pointsto(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '-')
    return join_token_nchar(lx, line, TK_DMINUS);

  if (ch == '>')
    return join_token_nchar(lx, line, TK_POINTSTO);

  return join_token(lx, line, TK_MINUS, NULL);
}


int
get_plus_dplus(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '+')
    return join_token_nchar(lx, line, TK_DPLUS);

  return join_token(lx, line, TK_PLUS, NULL);
}


int
get_and_dand(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '&')
    return join_token_nchar(lx, line, TK_DAND);

  return join_token(lx, line, TK_AND, NULL);
}


int
get_or_dor(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '|')
    return join_token_nchar(lx, line, TK_DOR);

  return join_token(lx, line, TK_DOR, NULL);
}


int
get_assign_eq(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '=')
    return join_token_nchar(lx, line, TK_EQ);

  return join_token(lx, line, TK_ASSIGN, NULL);
}


int
get_gt_geq(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '=')
    return join_token_nchar(lx, line, TK_GEQ);

  return join_token(lx, line, TK_GT, NULL);
}


int
get_lt_leq(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '=')
    return join_token_nchar(lx, line, TK_LEQ);

  return join_token(lx, line, TK_LT, NULL);
}


int
get_exclamation_neq(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '=')
    return join_token_nchar(lx, line, TK_NEQ);

  return join_token(lx, line, TK_EXCLAMATION, NULL);
}


void
jump_multi_comments_recur(struct lex *lx)
{
  int ch;

  for (; ch = lx->ch, ch != '*'; lx->nchar())
    assert_not_eof(lx, ch);

  ch = lx->nchar();
  if (ch != '/')
    jump_multi_comments_recur(lx);
}


int
jump_multi_comments(struct lex *lx)
{
  int ch;

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  jump_multi_comments_recur(lx);
  lx->nchar();
  return 1;
}


int
jump_line_comments(struct lex *lx)
{
  int ch;

  for (; ch = lx->ch, ch != '\n'; lx->nchar())
    assert_not_eof(lx, ch);

  return 1;
}


int
get_divide_or_jump_comments(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (ch == '*')
    return jump_multi_comments(lx);
  if (ch == '/')
    return jump_line_comments(lx);
  return join_token(lx, line, TK_DIVIDE, NULL);
}


int
get_numstr(struct lex *lx, int (*chkfn)(int), int line)
{
  char *buffer;
  int cnt;

  buffer = buff_tmp;
  *buffer++ = lx->ch;

  for (cnt = 1; cnt < MAX_CSTR_LENGTH && chkfn(lx->nchar()); cnt++)
    *buffer++ = lx->ch;

  *buffer = '\0';

  if (cnt > MAX_INT_LENGTH)
    exit_with_info("%s:%d:[LEXER]Number too long\n",
        lx->fname, line);

  return 1;
}


int
get_numval(struct lex *lx, int base, int (*cnvfn)(int), int line)
{
  char *buffer, *cmpstr;
  long i, j, k;

  buffer = buff_tmp;

  cmpstr = MAX_DECIMAL_STRING;
  if (base == 8)
    cmpstr = MAX_OCTAL_STRING;
  if (base == 16)
    cmpstr = MAX_HEX_STRING;

  j = slen(buffer);
  k = slen(cmpstr);
  if (j > k || (j == k && scmp(buffer, cmpstr) > 0))
    exit_with_info("%s:%d:[LEXER]Number too big\n",
        lx->fname, line);

  for (i = 0; *buffer; buffer++)
    i = i * base + cnvfn(*buffer);

  return i;
}


int
cnv_hexdigit(int ch)
{
  if (ch >= 'a' && ch <= 'f')
    return ch - 'a' + 10;
  if (ch >= 'A' && ch <= 'F')
    return ch - 'A' + 10;
  return ch - '0';
}


int
cnv_digit(int ch)
{
  return ch - '0';
}


int
get_octal(struct lex *lx, int line)
{
  long i;

  get_numstr(lx, check_octal, line);
  i = get_numval(lx, 8, cnv_digit, line);

  return join_token(lx, line, TK_CINT, (void *) i);
}


int
get_decimal(struct lex *lx, int line)
{
  long i;

  get_numstr(lx, check_decimal, line);
  i = get_numval(lx, 10, cnv_digit, line);

  return join_token(lx, line, TK_CINT, (void *) i);
}


int
get_hex(struct lex *lx, int line)
{
  long i, ch;

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  get_numstr(lx, check_hex, line);
  i = get_numval(lx, 16, cnv_hexdigit, line);

  return join_token(lx, line, TK_CINT, (void *) i);
}


int
get_integer(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->ch;
  assert_not_eof(lx, ch);

  if (ch != '0')
    return get_decimal(lx, line);

  ch = lx->nchar();
  if (check_octal(ch))
    return get_octal(lx, line);

  if (ch == 'x' || ch == 'X')
    return get_hex(lx, line);

  return join_token(lx, line, TK_CINT, (void *) 0);
}


int
get_identifier(struct lex *lx)
{
  char *buffer;
  int line, cnt, kw;

  line = lx->line;

  buffer = buff_tmp;
  *buffer++ = lx->ch;

  for (cnt = 1; cnt < MAX_CSTR_LENGTH && check_ident(lx->nchar()); cnt++)
    *buffer++ = lx->ch;

  *buffer = '\0';

  if (cnt > MAX_IDENT_LENGTH)
    exit_with_info("%s:%d:[LEXER]Identifier too long\n",
        lx->fname, line);

  kw = try_get_keyword(buff_tmp);
  if (kw)
    return join_token(lx, line, kw, NULL);

  return join_token(lx, line, TK_IDENT, copy_of_buffer(buff_tmp));
}


int
get_hex_escape(struct lex *lx)
{
  int r, t;

  t = lx->nchar();
  assert_not_eof(lx, t);
  assert_hex(lx, t);
  r = cnv_hexdigit(t) * 16;

  t = lx->nchar();
  assert_not_eof(lx, t);
  assert_hex(lx, t);
  r += cnv_hexdigit(t);

  lx->nchar();

  return r;
}


int
get_octal_escape(struct lex *lx)
{
  int ch, v;
  v = cnv_digit(lx->ch);

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  lx->nchar();
  return v;
}


int
get_normal_escape(struct lex *lx, int ch)
{
  int line;

  line = lx->line;
  lx->nchar();

  if (ch == 'a')
    return 7;
  if (ch == 'b')
    return 8;
  if (ch == 't')
    return 9;
  if (ch == 'n')
    return 10;
  if (ch == 'v')
    return 11;
  if (ch == 'f')
    return 12;
  if (ch == 'r')
    return 13;
  if (ch == '\\')
    return '\\';
  if (ch == '\'')
    return '\'';
  if (ch == '"')
    return '"';
  if (ch == '\n')
    return 0xff;

  exit_with_info("%s:%d:[LEXER]Unknown escape sequence: \\%c\n",
      lx->fname, line, ch);

  return ch;
}


int
get_escape_seq(struct lex *lx)
{
  int ch;
  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (check_octal(ch))
    return get_octal_escape(lx);
  if (ch == 'x')
    return get_hex_escape(lx);

  return get_normal_escape(lx, ch);
}


int
get_character(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = lx->nchar();

  assert_not_eof(lx, ch);

  if (ch == '\'')
    exit_with_info("%s:%d:[LEXER]Empty character literal\n",
        lx->fname, line);

  if (ch == '\\')
    ch = get_escape_seq(lx);
  else
    lx->nchar();

  if (ch == '\n')
    exit_with_info("%s:%d:[LEXER]Missing terminating \"'\"\n",
        lx->fname, line);

  if (ch == 0xff)
    exit_with_info("%s:%d:[LEXER]Invalid character\n",
        lx->fname, line);

  assert_ch(lx, lx->ch, '\'');

  join_token(lx, line, TK_CCHAR, (void *) ch);

  lx->nchar();
  return 1;
}


int
get_strchar_sub(struct lex *lx)
{
  char ch;

  ch = lx->ch;
  assert_not_eof(lx, ch);

  if (ch == '\n')
    exit_with_info("%s:%d:[LEXER]Missing terminating '\"'\n",
        lx->fname, lx->line - 1);

  if (ch == '"')
    return 0;

  if (ch == '\\')
    ch = get_escape_seq(lx);
  else
    lx->nchar();

  return ch;
}


int
get_strchar(struct lex *lx, char *c)
{
  char ch;

  ch = get_strchar_sub(lx);
  for (; ch == 0xff; )
    ch = get_strchar_sub(lx);

  *c = ch;

  if (ch == 0)
    return 0;
  else
    return 1;
}


int
get_string(struct lex *lx)
{
  char *buffer, ch;
  int line, cnt;

  line = lx->line;
  buffer = buff_tmp;

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  for (cnt = 0; cnt < MAX_CSTR_LENGTH && get_strchar(lx, &ch); cnt++)
    *buffer++ = ch;

  *buffer = '\0';

  if (cnt == MAX_CSTR_LENGTH)
    exit_with_info("%s:%d:[LEXER]String too long\n",
        lx->fname, line);

  join_token(lx, line, TK_CSTR, copy_of_buffer(buff_tmp));

  lx->nchar();
  return 1;
}


char *
header_filename(struct lex *lx, int line, char *s)
{
  char *a, *b;

  for (a = s + 7; *a && check_space(*a); a++);

  if (!*a || *a != '"')
    exit_with_info("%s:%d:[LEXER]Invalid include directive\n",
        lx->fname, line);
  a++;
  b = s;
  for (; check_ident(*a) || *a == '.'; )
    *b++ = *a++;

  *b = '\0';

  if (*a != '"')
    exit_with_info("%s:%d:[LEXER]Invalid include directive\n",
        lx->fname, line);

  a++;
  for (; *a && check_space(*a); a++);

  if (*a)
    exit_with_info("%s:%d:[LEXER]Invalid content after include\n",
        lx->fname, line);

  return s;
}


int
handle_header(struct lex *lx, int line, char *p)
{
  char *fname;

  fname = header_filename(lx, line, p);

  printf("will including file: %s...\n", fname);

  return 1;
}


int
handle_define(struct lex *lx, int line, char *p)
{
  return 1;
}


int
handle_ifndef(struct lex *lx, int line, char *p)
{
  return 1;
}


char *
preprocess_content(struct lex *lx)
{
  char ch, *buffer;
  int line;

  line = lx->line;
  buffer = buff_tmp;

  if ((lx->line == 1 && lx->pch != 0) || (lx->line != 1 && lx->pch != '\n'))
    exit_with_info("%s:%d:[LEXER]\"#\" should be the 1st ch of a line\n",
        lx->fname, line);

  ch = lx->nchar();
  assert_not_eof(lx, ch);

  if (check_space(ch))
    exit_with_info("%s:%d:[LEXER]Do not write space after \"#\"\n",
        lx->fname, line);

  for (; ch != '\n'; ch = lx->nchar(), assert_not_eof(lx, ch))
    *buffer++ = ch;

  *buffer = '\0';

  if (*(buffer - 1) == '\\')
    exit_with_info("%s:%d:[LEXER]Multiline in preprocess is invalid\n",
        lx->fname, line);

  return buff_tmp;
}


int
preprocess(struct lex *lx)
{
  char *buffer;
  int line;

  line = lx->line;
  buffer = preprocess_content(lx);

  if (!scmpn(buffer, "include", 7))
    return handle_header(lx, line, buffer);

  if (!scmpn(buffer, "define", 6))
    return handle_define(lx, line, buffer);

  if (!scmpn(buffer, "ifndef", 6))
    return handle_ifndef(lx, line, buffer);

  if (!scmpn(buffer, "endif", 5))
    return 1;

  exit_with_info("%s:%d:[LEXER]Preprocess directive error\n",
      lx->fname, line);

  return 1;
}


int
get_token(struct lex *lx)
{
  for (; check_space(lx->ch); )
    lx->nchar();
  if (lx->ch == EOF)
    return 0;
  if (lx->ch == ';')
    return join_token_nchar(lx, lx->line, TK_SEMICOLON);
  if (lx->ch == ':')
    return join_token_nchar(lx, lx->line, TK_COLON);
  if (lx->ch == ',')
    return join_token_nchar(lx, lx->line, TK_COMMA);
  if (lx->ch == '?')
    return join_token_nchar(lx, lx->line, TK_QUESTION);
  if (lx->ch == '^')
    return join_token_nchar(lx, lx->line, TK_CARET);
  if (lx->ch == '~')
    return join_token_nchar(lx, lx->line, TK_TILDE);
  if (lx->ch == '*')
    return join_token_nchar(lx, lx->line, TK_ASTERISK);
  if (lx->ch == '%')
    return join_token_nchar(lx, lx->line, TK_MOD);
  if (lx->ch == '{')
    return join_token_nchar(lx, lx->line, TK_BEGIN);
  if (lx->ch == '}')
    return join_token_nchar(lx, lx->line, TK_END);
  if (lx->ch == '[')
    return join_token_nchar(lx, lx->line, TK_OPENBR);
  if (lx->ch == ']')
    return join_token_nchar(lx, lx->line, TK_CLOSEBR);
  if (lx->ch == '(')
    return join_token_nchar(lx, lx->line, TK_OPENPA);
  if (lx->ch == ')')
    return join_token_nchar(lx, lx->line, TK_CLOSEPA);
  if (lx->ch == '/')
    return get_divide_or_jump_comments(lx);
  if (lx->ch == '!')
    return get_exclamation_neq(lx);
  if (lx->ch == '+')
    return get_plus_dplus(lx);
  if (lx->ch == '-')
    return get_minus_dminus_pointsto(lx);
  if (lx->ch == '.')
    return get_dot_ellipsis(lx);
  if (lx->ch == '&')
    return get_and_dand(lx);
  if (lx->ch == '|')
    return get_or_dor(lx);
  if (lx->ch == '=')
    return get_assign_eq(lx);
  if (lx->ch == '>')
    return get_gt_geq(lx);
  if (lx->ch == '<')
    return get_lt_leq(lx);
  if (lx->ch == '\'')
    return get_character(lx);
  if (lx->ch == '"')
    return get_string(lx);
  if (lx->ch == '#')
    return preprocess(lx);
  if (check_ident_start(lx->ch))
    return get_identifier(lx);
  if (check_decimal(lx->ch))
    return get_integer(lx);

  exit_with_info("%s:%d:[LEXER]Unknown char: [0x%x]\n",
      lx->fname, lx->line, lx->ch);

  return 1;
}


struct token *
tokenize_base(struct lex *lx)
{
  lx->nchar();
  for (; get_token(lx); );

  return lx->tk_s;
}


struct token *
tokenize()
{
  return tokenize_base(&lxsrc);
}


