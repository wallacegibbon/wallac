#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "vars.h"
#include "limits.h"
#include "misc.h"

char *copy_string(char *buffer)
{
	char *p;
	int i;

	i = strlen(buffer);
	if (i == -1)
		exit_with("copy_string, string too long\n");

	p = malloc(i + 1);
	if (!p)
		exit_with("copy_string, failed on malloc\n");

	strcpy(p, buffer);
	return p;
}

int is_valid_filename(char *filename)
{
	int i;

	i = strlen(filename);
	if (i == -1)
		exit_with("is_valid_filename, string(filename) too long\n");

	return i < MAX_FILENAME_SIZE - 3;
}

int upper_case(char ch)
{
	return ch & ~(1 << 5);
}

int init_pathname_out(char *pathname, char *out)
{
	int i, j, l;

	j = 0;
	for (i = 0, l = strlen(pathname); i < l; i++)
		if (*(pathname + i) == '/')
			j = i;

	if (j)
		strcpy(out, pathname + j + 1);
	else
		strcpy(out, pathname);

	for (; *out != '.' && *out;)
		out++;

	*out++ = '.';
	*out++ = 's';
	*out = '\0';

	return 1;
}

int purepath_of(char *orig, char *out)
{
	int i, j, l;

	j = 0;
	for (i = 0, l = strlen(orig); i < l; i++)
		if (*(orig + i) == '/')
			j = i;

	if (j)
		strncpy(out, orig, j + 1);
	else
		strcpy(out, "./");

	return 1;
}

char *mkpath_from(char *origpath, char *newfile)
{
	char *buff, *p;
	int i;

	buff = alloca(strlen(origpath));
	purepath_of(origpath, buff);

	i = strlen(buff);
	p = malloc(i + strlen(newfile) + 1);
	if (!p)
		exit_with("mkpath_from, failed on malloc\n");

	strcpy(p, buff);
	strcpy(p + i, newfile);

	return p;
}

void exit_with(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	exit(1);
}
