#include "assertch.h"
#include "checkch.h"


int
check_identifier(int ch)
{
  assert_not_eof(ch);
  return check_alpha(ch) || check_decimal(ch) || ch == '_';
}


int
check_identifier_start(int ch)
{
  assert_not_eof(ch);
  return check_alpha(ch) || ch == '_';
}


int
check_string_end(int ch)
{
  assert_not_ch(ch, '\r');
  assert_not_ch(ch, '\n');

  return ch == '"';
}


int
check_alpha(int ch)
{
  assert_not_eof(ch);
  return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}


int
check_a2f(int ch)
{
  assert_not_eof(ch);
  return ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F';
}


int
check_octal(int ch)
{
  assert_not_eof(ch);
  return ch >= '0' && ch <= '7';
}


int
check_decimal(int ch)
{
  assert_not_eof(ch);
  return ch >= '0' && ch <= '9';
}


int
check_hex(int ch)
{
  assert_not_eof(ch);
  return check_decimal(ch) || check_a2f(ch);
}


int
check_space(int ch)
{
  return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}
