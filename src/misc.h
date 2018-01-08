#ifndef __MISC_H__
#define __MISC_H__


#define UPPER(ch) ((ch) & ~(1 << 5))

void
init_outputname(char *outname, char *inname);

void
exit_with_info(char *info, ...);

char *
copy_of_buffer(char *buffer);


#endif
