#ifndef __CMDARGS_H_
#define __CMDARGS_H_


int
walk_arguments(char **argv, char **end);

int
handle_outfile_argument(char **argv, char **end);

int
handle_infile_argument(char **argv, char **end);


#endif
