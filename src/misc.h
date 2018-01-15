#ifndef __MISC_H__
#define __MISC_H__


char *
copy_of_buffer(char *buffer);

int
is_valid_filename(char *filename);

int
upper_case(char ch);

int
init_pathname_out(char *pathname, char *out);

int
init_srcpath(char *filename, char *srcpath);

void
exit_with(char *info, ...);


#endif
