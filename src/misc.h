#ifndef __MISC_H__
#define __MISC_H__


int
scmpn(char *s1, char *s2, int n);

int
scmp(char *s1, char *s2);

int
scpyn(char *dst, char *src, int n);

int
scpy(char *dst, char *src);

int
slen(char *s);

char *
copy_of_buffer(char *buffer);

int
is_valid_filename(char *filename);

int
upper_case(char ch);

void
init_outputname(char *outname, char *inname);

void
exit_with_info(char *info, ...);


#endif
