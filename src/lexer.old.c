#include "vars.h"
#include "misc.h"
#include "token.h"
#include "limit.h"


#define CHK_SPACE(ch) (ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ')
#define CHK_DIGIT(ch) (ch >= '0' && ch <= '9')
#define UPPER(ch) (ch & ~(1 << 5))
#define CHK_HEX(ch) (UPPER(ch) >= 'A' && UPPER(ch) <= 'F')

#define CHK_IDENT_START(ch) \
  ((UPPER(ch) >= 'A' && UPPER(ch) <= 'Z') || ch == '_')
#define CHK_IDENT_BODY(ch) (CHK_IDENT_START(ch) || CHK_DIGIT(ch))




int
hex_to_num(int ch)
{
  int n = CHK_DIGIT(ch) ? ch-'0' : CHK_HEX(ch) ? UPPER(ch)-'A'+10 : 0xff;
  if (n < 0 || n > 15)
    exit_with_info("[%d][LEXER]Invalid hex digits:%x\n", current_line, ch);
  return n;
}


int
get_ident()
{
  char c = current_ch, *p = ident_buff;
  int i;
  do
  {
    if (i == MAX_IDENT_LENGTH - 1)
      exit_with_info("[%d][LEXER]identifier too long\n", current_line);

    *(p + i++) = c;

    c = next_char();
    if (c == EOF)
      exit_with_info("[%d][LEXER]File incomplete\n", current_line);
  }
  while (CHK_IDENT_BODY(c));

  *(p + i) = '\0';

  printf("get_ident result: %s\n", ident_buff);

  return TK_IDENT;
}


int
get_decimal()
{
  int v = 0;
  while (CHK_DIGIT(current_ch))
  {
    v *= 10;
    v += current_ch - '0';
    next_char();
  }

  return v;
}


int
get_octal()
{
  int v = 0;
  while (current_ch >= '0' && current_ch <= '7')
  {
    v *= 8;
    v += current_ch - '0';
    next_char();
  }

  return v;
}

int
get_hex()
{
  int v = 0;
  if (current_ch == EOF)
    exit_with_info("[%d][LEXER]Unfinished hex int", current_line);

  while (CHK_DIGIT(current_ch) || CHK_HEX(current_ch))
  {
    v *= 16;
    v += hex_to_num(current_ch);
    next_char();
  }

  return v;
}


int
get_int()
{
  int v;
  char c = current_ch;

  if (current_ch == '0')
  {
    next_char();
    if (current_ch == EOF)
      exit_with_info("[%d][LEXER]Unfinished int", current_line);

    if (UPPER(current_ch) == 'X')
    {
      next_char();
      v = get_hex();
    }
    else
    {
      v = get_octal();
    }
  }
  else
  {
    v = get_decimal();
  }

  printf("get_int result: 0X%02X, %d\n", v, v);

  return TK_CINT;
}


int
get_hex_seq()
{
  char a = next_char(), b = next_char();
  if (a == EOF || b == EOF)
    exit_with_info("[%d][LEXER]Unfinished hex digits\n", current_line);
  return hex_to_num(a) * 16 + hex_to_num(b);
}


int
get_escape_seq()
{
  char ch = next_char();
  if (ch == EOF)
    exit_with_info("[%d][LEXER]Unfinished escape seq\n", current_line);

  switch (ch)
  {
  case 'x':
    return get_hex_seq();
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
get_char()
{
  char c = next_char(), d;

  if (c == EOF)
    exit_with_info("[%d][LEXER]Unfinished char\n", current_line);

  if (c == '\\')
    c = get_escape_seq();

  d = next_char();

  if (d == EOF)
    exit_with_info("[%d][LEXER]Unfinished char\n", current_line);

  if (d != '\'')
    exit_with_info("[%d][LEXER]Missing terminating \"'\"\n", current_line);

  printf("get_char result: %02X, %c\n", c, c);

  next_char();

  return TK_CCHAR;
}


int
get_str()
{
  return TK_CSTR;
}


int
get_token()
{
  if (!start_tk)
    start_tk = malloc(sizeof(struct node));

  if (!start_tk)
    exit_with_info("Failed alloc memory for start_tk\n");

  if (!current_tk)
    current_tk = start_tk;

  while (CHK_SPACE(current_ch))
    next_char();

  printf("Current ch:%2X\n", current_ch);

  if (CHK_IDENT_START(current_ch))
    return get_ident();

  if (CHK_DIGIT(current_ch))
    return get_int();

  if (current_ch == '"')
    return get_str();

  if (current_ch == '\'')
    return get_char();

  if (current_ch == EOF)
    return TK_EOF;

  exit_with_info("Unknown ch: %x\n", current_ch);

  return 0;
}


int
tokenize()
{
  next_char();

  while (get_token() != TK_EOF);

  return 0;
}

