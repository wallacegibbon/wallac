#ifndef __MISC_H__
#define __MISC_H__


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
