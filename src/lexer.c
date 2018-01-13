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




int
file_nextchar(struct lex *lx)
{
  int ch;

  ch = fgetc(lx->fp);
  if (ch == EOF && ferror(lx->fp))
    exit_with("Failed reading %s(%d)\n", lx->fname, errno);

  if (lx->line >= INT_MAX)
    exit_with("File %s too long\n", lx->fname);

  if (ch == '\n')
    lx->line++;

  lx->pch = lx->ch;
  lx->ch = ch;
  lx->cursor++;

  return ch;
}


int
str_nextchar(struct lex *lx)
{
  int ch;

  ch = *(lx->str + lx->cursor);
  if (ch)
    lx->cursor++;
  else
    ch = EOF;

  lx->pch = lx->ch;
  lx->ch = ch;

  return ch;
}


int
nextchar(struct lex *lx)
{
  if (lx->fp)
    return file_nextchar(lx);
  if (lx->str)
    return str_nextchar(lx);

  exit_with("nextchar error\n");
}


struct lex *
new_lexer_common(char *buff, struct hashtbl *mtbl)
{
  struct lex *lx;

  lx = malloc(sizeof(struct lex));
  if (!lx)
    exit_with("Failed alloc memory for lex\n");

  lx->tk_s = new_token(0, NULL);
  lx->tk_s->prev = NULL;
  lx->tk_s->next = NULL;
  lx->tk_c = lx->tk_s;

  lx->fname = NULL;
  lx->fp = NULL;
  lx->str = NULL;

  lx->buff = buff;
  lx->mtbl = mtbl;

  lx->line = 1;
  lx->ch = '\0';
  lx->pch = '\0';
  lx->cursor = 0;

  return lx;
}


struct lex *
new_lexer_file(char *fname, char *buff, struct hashtbl *mtbl)
{
  struct lex *lx;
  lx = new_lexer_common(buff, mtbl);

  lx->fname = fname;
  lx->fp = fopen(fname, "r");
  if (!lx->fp)
    exit_with("Failed opening %s\n", fname);

  return lx;
}


struct lex *
new_lexer_str(char *str, char *buff, struct hashtbl *mtbl)
{
  struct lex *lx;
  lx = new_lexer_common(buff, mtbl);

  lx->str = str;
  return lx;
}


void
free_lexer(struct lex *lx)
{
  if (lx->fname)
    fclose(lx->fp);
  free(lx);
}


int
join_token(struct lex *lx, int line, int type, void *p)
{
  struct token *t;
  t = new_token(type, p);
  t->line = line;
  t->prev = lx->tk_c;
  lx->tk_c->next = t;
  lx->tk_c = t;

  return 1;
}


int
join_token_nchar(struct lex *lx, int line, int type)
{
  join_token(lx, line, type, NULL);
  nextchar(lx);
  return 1;
}


int
join_chain(struct lex *lx, int line, struct token *orig)
{
  struct token *p, *pp;

  if (!orig)
    return 1;

  p = copy_token_chain(orig);
  if (!p)
    return 1;

  p->prev = lx->tk_c;
  lx->tk_c->next = p;

  for (; p; pp = p, p = p->next)
    p->line = line;

  lx->tk_c = pp;

  return 1;
}


int
get_ellipsis(struct lex *lx, int line)
{
  int ch;

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (ch != '.')
    exit_with("%s:%d:[LEXER]Unsupported element \"..\"\n",
        lx->fname, line);

  join_token(lx, line, TK_ELLIPSIS, NULL);
  nextchar(lx);
  return 1;
}


int
get_dot_ellipsis(struct lex *lx)
{
  int line, ch; 

  line = lx->line;
  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (ch == '.')
    return get_ellipsis(lx, line);

  if (check_decimal(ch))
    exit_with("%s:%d:[LEXER]Float literal is not supported\n",
        lx->fname, line);

  return join_token(lx, line, TK_DOT, NULL);
}


int
get_minus_dminus_pointsto(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
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
  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (ch == '=')
    return join_token_nchar(lx, line, TK_NEQ);

  return join_token(lx, line, TK_EXCLAMATION, NULL);
}


void
jump_multi_comments_recur(struct lex *lx)
{
  int ch;

  for (; ch = lx->ch, ch != '*'; nextchar(lx))
    assert_not_eof(lx, ch);

  ch = nextchar(lx);
  if (ch != '/')
    jump_multi_comments_recur(lx);
}


int
jump_multi_comments(struct lex *lx)
{
  int ch;

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  jump_multi_comments_recur(lx);
  nextchar(lx);
  return 1;
}


int
skip_line(struct lex *lx)
{
  int ch;

  for (; ch = lx->ch, ch != '\n'; nextchar(lx))
    assert_not_eof(lx, ch);

  nextchar(lx);
  return 1;
}


int
get_divide_or_jump_comments(struct lex *lx)
{
  int line, ch;

  line = lx->line;
  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (ch == '*')
    return jump_multi_comments(lx);
  if (ch == '/')
    return skip_line(lx);
  return join_token(lx, line, TK_DIVIDE, NULL);
}


int
get_numstr(struct lex *lx, int (*chkfn)(int), int line)
{
  char *buffer;
  int cnt;

  buffer = lx->buff;
  *buffer++ = lx->ch;

  for (cnt = 1; cnt < MAX_CSTR_LENGTH && chkfn(nextchar(lx)); cnt++)
    *buffer++ = lx->ch;

  *buffer = '\0';

  if (cnt > MAX_INT_LENGTH)
    exit_with("%s:%d:[LEXER]Number too long\n",
        lx->fname, line);

  return 1;
}


int
get_numval(struct lex *lx, int base, int (*cnvfn)(int), int line)
{
  char *buffer, *cmpstr;
  long i, j, k;

  buffer = lx->buff;

  cmpstr = MAX_DECIMAL_STRING;
  if (base == 8)
    cmpstr = MAX_OCTAL_STRING;
  if (base == 16)
    cmpstr = MAX_HEX_STRING;

  j = slen(buffer);
  k = slen(cmpstr);
  if (j > k || (j == k && scmp(buffer, cmpstr) > 0))
    exit_with("%s:%d:[LEXER]Number too big\n",
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

  ch = nextchar(lx);
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

  ch = nextchar(lx);
  if (check_octal(ch))
    return get_octal(lx, line);

  if (ch == 'x' || ch == 'X')
    return get_hex(lx, line);

  return join_token(lx, line, TK_CINT, (void *) 0);
}


int
get_identifier(struct lex *lx)
{
  struct hashnode *kv;
  char *buffer;
  int line, cnt, kw;

  line = lx->line;

  buffer = lx->buff;
  *buffer++ = lx->ch;

  for (cnt = 1; cnt < MAX_CSTR_LENGTH && check_ident(nextchar(lx)); cnt++)
    *buffer++ = lx->ch;

  *buffer = '\0';

  if (cnt > MAX_IDENT_LENGTH)
    exit_with("%s:%d:[LEXER]Identifier too long\n",
        lx->fname, line);

  buffer = lx->buff;

  kw = try_get_keyword(buffer);
  if (kw)
    return join_token(lx, line, kw, NULL);

  kv = hash_get(lx->mtbl, buffer);
  if (kv)
    return join_chain(lx, line, (struct token *) kv->value);

  return join_token(lx, line, TK_IDENT, copy_of_buffer(buffer));
}


int
get_hex_escape(struct lex *lx)
{
  int r, t;

  t = nextchar(lx);
  assert_not_eof(lx, t);
  assert_hex(lx, t);
  r = cnv_hexdigit(t) * 16;

  t = nextchar(lx);
  assert_not_eof(lx, t);
  assert_hex(lx, t);
  r += cnv_hexdigit(t);

  nextchar(lx);

  return r;
}


int
get_octal_escape(struct lex *lx)
{
  int ch, v;
  v = cnv_digit(lx->ch);

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  nextchar(lx);
  return v;
}


int
get_normal_escape(struct lex *lx, int ch)
{
  int line;

  line = lx->line;
  nextchar(lx);
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
  exit_with("%s:%d:[LEXER]Unknown escape sequence: \\%c\n",
      lx->fname, line, ch);

  return ch;
}


int
get_escape_seq(struct lex *lx)
{
  int ch;
  ch = nextchar(lx);
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
  ch = nextchar(lx);

  assert_not_eof(lx, ch);

  if (ch == '\'')
    exit_with("%s:%d:[LEXER]Empty character literal\n",
        lx->fname, line);

  if (ch == '\\')
    ch = get_escape_seq(lx);
  else
    nextchar(lx);

  if (ch == '\n')
    exit_with("%s:%d:[LEXER]Missing terminating \"'\"\n",
        lx->fname, line);

  if (ch == 0xff)
    exit_with("%s:%d:[LEXER]Invalid character\n",
        lx->fname, line);

  assert_ch(lx, lx->ch, '\'');

  join_token(lx, line, TK_CCHAR, (void *) ch);

  nextchar(lx);
  return 1;
}


int
get_strchar_sub(struct lex *lx)
{
  char ch;

  ch = lx->ch;
  assert_not_eof(lx, ch);

  if (ch == '\n')
    exit_with("%s:%d:[LEXER]Missing terminating '\"'\n",
        lx->fname, lx->line - 1);

  if (ch == '"')
    return 0;

  if (ch == '\\')
    ch = get_escape_seq(lx);
  else
    nextchar(lx);

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
  buffer = lx->buff;

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  for (cnt = 0; cnt < MAX_CSTR_LENGTH && get_strchar(lx, &ch); cnt++)
    *buffer++ = ch;

  *buffer = '\0';

  if (cnt == MAX_CSTR_LENGTH)
    exit_with("%s:%d:[LEXER]String too long\n",
        lx->fname, line);

  join_token(lx, line, TK_CSTR, copy_of_buffer(lx->buff));

  nextchar(lx);
  return 1;
}


char *
header_filename(struct lex *lx, int line, char *s)
{
  char *a, *b;

  for (a = s; *a && check_space(*a); a++);

  if (!*a || *a != '"')
    exit_with("%s:%d:[LEXER]Invalid include directive\n",
        lx->fname, line);
  a++;
  for (b = s; check_ident(*a) || *a == '.'; )
    *b++ = *a++;

  *b = '\0';

  if (*a != '"')
    exit_with("%s:%d:[LEXER]Invalid include directive\n",
        lx->fname, line);

  a++;
  for (; *a && check_space(*a); a++);

  if (*a)
    exit_with("%s:%d:[LEXER]Invalid content after include\n",
        lx->fname, line);

  return s;
}


int
handle_header(struct lex *lx, int line, char *s)
{
  struct token *tks;
  struct lex *ilx;
  char *fname, *p;
  int i;

  fname = header_filename(lx, line, s + 7);
  i = slen(path_src);

  p = malloc(i + slen(fname));
  if (!p)
    exit_with("Failed alloc memory for new header file\n");

  scpy(p, path_src);
  scpy(p + i, fname);

  if (debug)
    printf("Including file %s...\n", p);

  ilx = new_lexer_file(p, lx->buff, lx->mtbl);
  tks = tokenize_base(ilx);
  free_lexer(ilx);

  if (tks)
    join_chain(lx, line, tks->next);

  return 1;
}


char *
shift_macroname(struct lex *lx, int line, char *s, int offset)
{
  char *a, *b, *r;
  int i;

  a = s + offset;
  if (!check_space(*a))
    exit_with("%s:%d:[LEXER]Invalid define directive\n",
        lx->fname, line);

  for (; *a && check_space(*a); a++);

  if (!check_ident_start(*a))
    exit_with("%s:%d:[LEXER]Invalid macro name\n",
        lx->fname, line);

  b = a;
  for (i = 0; check_ident(*a); a++)
    i++;

  r = malloc(i);
  if (!r)
    exit_with("%s:%d:[LEXER]Failed alloc memory for macro name\n",
        lx->fname, line);

  scpyn(r, b, i);
  scpy(s, a);

  return r;
}


int
handle_define(struct lex *lx, int line, char *s)
{
  struct token *tks;
  struct lex *slx;
  char *name;
  int i;

  name = shift_macroname(lx, line, s, 6);

  slx = new_lexer_str(s, lx->buff, lx->mtbl);
  slx->line = line;

  tks = tokenize_base(slx);
  free_lexer(slx);

  i = hash_put(lx->mtbl, name, (void *) tks->next);
  if (!i)
    exit_with("%s:%d[LEXER]macro %s is already defined\n",
        lx->fname, line, name);

  return 1;
}


int
skip_until_endif(struct lex *lx)
{
  char ch, *buffer;
  int i;

  for (; ch = lx->ch, ch != '#'; skip_line(lx));

  buffer = lx->buff;
  for (i = 0; ch = nextchar(lx), assert_not_eof(lx, ch) && i < 5; i++)
   *buffer++ = ch;
  *buffer = '\0';

  if (!scmp("endif", lx->buff))
    return skip_line(lx);
  else
    return skip_until_endif(lx);
}


int
handle_ifndef(struct lex *lx, int line, char *s)
{
  char *name;

  name = shift_macroname(lx, line, s, 6);

  if (hash_keyexist(lx->mtbl, name))
    return skip_until_endif(lx);
  else
    return 1;
}


char *
preprocess_content(struct lex *lx)
{
  char ch, *buffer;
  int line;

  buffer = lx->buff;
  line = lx->line;

  if ((lx->line == 1 && lx->pch != 0) || (lx->line != 1 && lx->pch != '\n'))
    exit_with("%s:%d:[LEXER]\"#\" should be the 1st ch of a line\n",
        lx->fname, line);

  ch = nextchar(lx);
  assert_not_eof(lx, ch);

  if (check_space(ch))
    exit_with("%s:%d:[LEXER]Do not write space after \"#\"\n",
        lx->fname, line);

  for (; ch != '\n'; ch = nextchar(lx), assert_not_eof(lx, ch))
    *buffer++ = ch;

  *buffer = '\0';

  if (*(buffer - 1) == '\\')
    exit_with("%s:%d:[LEXER]Multiline in preprocess is invalid\n",
        lx->fname, line);

  nextchar(lx);

  return lx->buff;
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

  exit_with("%s:%d:[LEXER]Preprocess directive error\n",
      lx->fname, line);

  return 1;
}


int
get_token(struct lex *lx)
{
  for (; check_space(lx->ch); )
    nextchar(lx);
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

  exit_with("%s:%d:[LEXER]Unknown char (0x%x)\n",
      lx->fname, lx->line, lx->ch);

  return 1;
}


struct token *
tokenize_base(struct lex *lx)
{
  nextchar(lx);
  for (; get_token(lx); );

  return lx->tk_s;
}


struct token *
tokenize()
{
  struct hashtbl *macrotbl;
  struct lex *lx;
  char *buff;

  buff = malloc(MAX_CSTR_LENGTH);
  if (!buff)
    exit_with("Failed alloc memory for lex buffer\n");

  macrotbl = new_hashtbl(20);

  lx = new_lexer_file(filename_src, buff, macrotbl);

  return tokenize_base(lx);
}


