#ifndef __MISC_H__
#define __MISC_H__

char *copy_string(char *buffer);

int is_valid_filename(char *filename);

int upper_case(char ch);

int init_pathname_out(char *pathname, char *out);

char *mkpath_from(char *origpath, char *newfile);

void exit_with(char *info, ...);

#endif
