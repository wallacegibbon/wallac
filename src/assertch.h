#ifndef __ASSERT_H__
#define __ASSERT_H__

void assert_not_eof(char ch);
void assert_not_ch(char ch, char unexpected_ch);
void assert_ch(char ch, char expected_ch);
void assert_octal(char ch);
void assert_decimal(char ch);
void assert_hex(char ch);
void assert_ident(char ch);

#endif
