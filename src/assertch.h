#ifndef __ASSERT_H__
#define __ASSERT_H__

int assert_not_eof(char ch);
int assert_not_ch(char ch, char unexpected_ch);
int assert_ch(char ch, char expected_ch);
int assert_octal(char ch);
int assert_decimal(char ch);
int assert_hex(char ch);
int assert_ident(char ch);

#endif
