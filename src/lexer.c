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


char buff_tmp[MAX_CSTR_LENGTH];
int current_line, current_ch;


int
next_char()
{
  current_ch = fgetc(fp_in);
  if (current_ch == EOF && ferror(fp_in))
    exit_with_info("Failed reading input file, %d\n", errno);

  if (current_line >= INT_MAX)
    exit_with_info("File too long\n");

  if (current_ch == '\n')
    current_line++;

  return current_ch;
}


void
close_input_file()
{
  int i;

  i = fclose(fp_in);
  if (i == EOF && ferror(fp_in))
    exit_with_info("Failed closing input file, %d\n", errno);
}


int
join_token_nchar(int line, int type)
{
  join_token(line, type, NULL);
  next_char();
  return 1;
}


int
get_ellipsis(int line)
{
  if (next_char() != '.')
    exit_with_info("%s:%d:[LEXER]Unsupported element \"..\"\n",
        filename, current_line);

  join_token(line, TK_ELLIPSIS, NULL);
  next_char();
  return 1;
}


int
get_dot_ellipsis()
{
  int line; 
  line = current_line;
  if (next_char() == '.')
    return get_ellipsis(line);

  if (check_decimal(current_ch))
    exit_with_info("%s:%d:[LEXER]Float literal is not supported\n",
        filename, current_line);

  join_token(line, TK_DOT, NULL);
  return 1;
}


int
get_minus_dminus_pointsto()
{
  int line;
  line = current_line;
  next_char();
  if (current_ch == '-')
    return join_token_nchar(line, TK_DMINUS);

  if (current_ch == '>')
    return join_token_nchar(line, TK_POINTSTO);

  join_token(line, TK_MINUS, NULL);
  return 1;
}


int
get_plus_dplus()
{
  int line;
  line = current_line;
  next_char();
  if (current_ch == '+')
    return join_token_nchar(line, TK_DPLUS);

  join_token(line, TK_PLUS, NULL);
  return 1;
}


int
get_and_dand()
{
  int line;
  line = current_line;
  if (next_char() == '&')
    return join_token_nchar(line, TK_DAND);

  join_token(line, TK_AND, NULL);
  return 1;
}


int
get_or_dor()
{
  int line;
  line = current_line;
  if (next_char() == '|')
    return join_token_nchar(line, TK_DOR);

  join_token(line, TK_DOR, NULL);
  return 1;
}


int
get_assign_eq()
{
  int line;
  line = current_line;
  if (next_char() == '=')
    return join_token_nchar(line, TK_EQ);

  join_token(line, TK_ASSIGN, NULL);
  return 1;
}


int
get_gt_geq()
{
  int line;
  line = current_line;
  if (next_char() == '=')
    return join_token_nchar(line, TK_GEQ);

  join_token(line, TK_GT, NULL);
  return 1;
}


int
get_lt_leq()
{
  int line;
  line = current_line;
  if (next_char() == '=')
    return join_token_nchar(line, TK_LEQ);

  join_token(line, TK_LT, NULL);
  return 1;
}


int
get_exclamation_neq()
{
  int line;
  line = current_line;
  if (next_char() == '=')
    return join_token_nchar(line, TK_NEQ);

  join_token(line, TK_EXCLAMATION, NULL);
  return 1;
}


void
jump_multi_comments_recur()
{
  while (current_ch != '*')
    next_char();

  if (next_char() != '/')
    jump_multi_comments_recur();
}


int
jump_multi_comments()
{
  next_char();
  jump_multi_comments_recur();
  next_char();
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
  int line, ch;

  line = current_line;
  ch = next_char();
  if (ch == '*')
    return jump_multi_comments();
  if (ch == '/')
    return jump_line_comments();
  join_token(line, TK_DIVIDE, NULL);
  return 1;
}


void
get_numstr(int (*chkfn)(int), int line)
{
  char *buffer;
  int cnt;

  buffer = buff_tmp;
  *buffer++ = current_ch;

  cnt = 1;
  while (cnt++ < MAX_INT_LENGTH-1 && chkfn(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_INT_LENGTH)
    exit_with_info("%s:%d:[LEXER]Number too long\n",
        filename, line);

  *buffer = '\0';
}


int
get_numval(int base, int (*cnvfn)(int), int line)
{
  char *buffer, *cmpstr;
  long i, j, k;

  buffer = buff_tmp;

  cmpstr = MAX_DECIMAL_STRING;
  if (base == 8)
    cmpstr = MAX_OCTAL_STRING;
  if (base == 16)
    cmpstr = MAX_HEX_STRING;

  j = strlen(buffer);
  k = strlen(cmpstr);
  if (j > k || (j == k && strcmp(buffer, cmpstr) > 0))
    exit_with_info("%s:%d:[LEXER]Number too big\n",
        filename, line);

  i = 0;
  while (*buffer)
    i = i * base + cnvfn(*buffer++);

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
get_octal(int line)
{
  long i;

  get_numstr(check_octal, line);
  i = get_numval(8, cnv_digit, line);

  join_token(line, TK_CINT, (void *) i);
  return 1;
}


int
get_decimal(int line)
{
  long i;

  get_numstr(check_decimal, line);
  i = get_numval(10, cnv_digit, line);

  join_token(line, TK_CINT, (void *) i);
  return 1;
}


int
get_hex(int line)
{
  long i;

  next_char();
  get_numstr(check_hex, line);
  i = get_numval(16, cnv_hexdigit, line);

  join_token(line, TK_CINT, (void *) i);
  return 1;
}


int
get_integer()
{
  int line, ch;

  line = current_line;
  ch = current_ch;

  if (ch != '0')
    return get_decimal(line);

  ch = next_char();
  if (check_octal(ch))
    return get_octal(line);

  if (UPPER(ch) == 'X')
    return get_hex(line);

  join_token(line, TK_CINT, (void *) 0);
  return 1;
}


int
get_identifier()
{
  char *buffer;
  int line, cnt, type;

  line = current_line;
  buffer = buff_tmp;
  *buffer++ = current_ch;

  cnt = 1;
  while (cnt++ < MAX_IDENT_LENGTH-1 && check_identifier(next_char()))
    *buffer++ = current_ch;

  if (cnt == MAX_IDENT_LENGTH)
    exit_with_info("%s:%d:[LEXER]Identifier too long\n",
        filename, current_line);

  *buffer = '\0';

  type = try_get_keyword(buff_tmp);
  if (!type)
    join_token(line, TK_IDENT, copy_of_buffer(buff_tmp));
  else
    join_token(line, type, NULL);

  return 1;
}


int
get_hex_escape()
{
  int r, t;

  t = next_char();
  assert_not_eof(t);
  assert_hex(t);
  r = cnv_hexdigit(t) * 16;

  t = next_char();
  assert_not_eof(t);
  assert_hex(t);
  r += cnv_hexdigit(t);

  next_char();

  return r;
}


int
get_octal_escape()
{
  int ch, v;
  v = cnv_digit(current_ch);
  ch = next_char();
  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  ch = next_char();
  if (!check_octal(ch))
    return v;
  v *= 8;
  v += cnv_digit(ch);

  next_char();
  return v;
}


int
get_normal_escape(int ch)
{
  next_char();
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
  if (ch == '0')
    return 0;
  return ch;
}


int
get_escape_seq()
{
  int ch;
  ch = next_char();
  assert_not_eof(ch);

  if (check_octal(ch))
    return get_octal_escape();
  if (ch == 'x')
    return get_hex_escape();

  return get_normal_escape(ch);
}


int
get_character()
{
  int line, ch;

  line = current_line;
  ch = next_char();

  assert_not_eof(ch);
  assert_not_ch(ch, '\'');

  if (ch == '\\')
    ch = get_escape_seq();
  else
    next_char();

  assert_ch(current_ch, '\'');

  join_token(line, TK_CCHAR, (void *) ch);

  next_char();
  return 1;
}


int
get_strchar(char *c)
{
  char ch;

  ch = current_ch;
  assert_not_eof(ch);
  assert_not_ch(ch, '\n');

  if (ch == '"')
    return 0;

  if (ch == '\\')
    ch = get_escape_seq();
  else
    next_char();

  *c = ch;
  return 1;
}


int
get_string()
{
  char *buffer, ch;
  int line, cnt;

  line = current_line;
  buffer = buff_tmp;

  next_char();

  cnt = 0;
  while (cnt++ < MAX_CSTR_LENGTH-1 && get_strchar(&ch))
    *buffer++ = ch;

  if (cnt == MAX_CSTR_LENGTH)
    exit_with_info("%s:%d:[LEXER]String too long\n",
        filename, current_line);

  *buffer = '\0';

  join_token(line, TK_CSTR, copy_of_buffer(buff_tmp));

  next_char();
  return 1;
}


int
get_token()
{
  while (check_space(current_ch))
    next_char();
  if (current_ch == EOF)
    return 0;
  if (current_ch == ';')
    return join_token_nchar(current_line, TK_SEMICOLON);
  if (current_ch == ':')
    return join_token_nchar(current_line, TK_COLON);
  if (current_ch == ',')
    return join_token_nchar(current_line, TK_COMMA);
  if (current_ch == '?')
    return join_token_nchar(current_line, TK_QUESTION);
  if (current_ch == '^')
    return join_token_nchar(current_line, TK_CARET);
  if (current_ch == '~')
    return join_token_nchar(current_line, TK_TILDE);
  if (current_ch == '*')
    return join_token_nchar(current_line, TK_ASTERISK);
  if (current_ch == '%')
    return join_token_nchar(current_line, TK_MOD);
  if (current_ch == '{')
    return join_token_nchar(current_line, TK_BEGIN);
  if (current_ch == '}')
    return join_token_nchar(current_line, TK_END);
  if (current_ch == '[')
    return join_token_nchar(current_line, TK_OPENBR);
  if (current_ch == ']')
    return join_token_nchar(current_line, TK_CLOSEBR);
  if (current_ch == '(')
    return join_token_nchar(current_line, TK_OPENPA);
  if (current_ch == ')')
    return join_token_nchar(current_line, TK_CLOSEPA);
  if (current_ch == '/')
    return get_divide_or_jump_comments();
  if (current_ch == '!')
    return get_exclamation_neq();
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
  if (current_ch == '\'')
    return get_character();
  if (current_ch == '"')
    return get_string();
  if (check_identifier_start(current_ch))
    return get_identifier();
  if (check_decimal(current_ch))
    return get_integer();

  exit_with_info("%s:%d:[LEXER]Unknown char: [0X%02X]\n",
      filename, current_line, current_ch);

  return 1;
}


int
tokenize()
{
  initialize_token_list();

  current_line = 1;
  next_char();
  while (get_token());

  close_input_file();
  return 0;
}


