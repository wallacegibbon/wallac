#include "assertch.h"
#include "checkch.h"


int
check_identifier(char ch)
{
  assert_not_eof(ch);
  return check_alpha(ch) || check_decimal(ch) || ch == '_';
}


int
check_identifier_start(char ch)
{
  assert_not_eof(ch);
  return check_alpha(ch) || ch == '_';
}


int
check_string_end(char ch)
{
  assert_not_ch(ch, '\r');
  assert_not_ch(ch, '\n');

  return ch == '"';
}


int
check_alpha(char ch)
{
  assert_not_eof(ch);
  return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}


int
check_a2f(char ch)
{
  assert_not_eof(ch);
  return ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F';
}


int
check_octal(char ch)
{
  assert_not_eof(ch);
  return ch >= '0' && ch <= '7';
}


int
check_decimal(char ch)
{
  assert_not_eof(ch);
  return ch >= '0' && ch <= '9';
}


int
check_hex(char ch)
{
  assert_not_eof(ch);
  return check_decimal(ch) || check_a2f(ch);
}


int
check_space(char ch)
{
  return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}
