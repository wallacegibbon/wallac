#include "checkch.h"

int check_ident(int ch)
{
	return check_alpha(ch) || ch == '_' || check_decimal(ch);
}

int check_ident_start(int ch)
{
	return check_alpha(ch) || ch == '_';
}

int check_alpha(int ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}

int check_a2f(int ch)
{
	return ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F';
}

int check_octal(int ch)
{
	return ch >= '0' && ch <= '7';
}

int check_decimal(int ch)
{
	return ch >= '0' && ch <= '9';
}

int check_hex(int ch)
{
	return check_decimal(ch) || check_a2f(ch);
}

int check_space(int ch)
{
	return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}
