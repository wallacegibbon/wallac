#ifndef __CVARIABLE_H__
#define __CVARIABLE_H__


struct cvars { char *name; int type; struct cvars *next; };
struct cstruct { char *name; struct cvars *vars; struct cstruct *next; };

extern struct cvars *global_vars;
extern struct cstruct *struct_defs;


int
find_variable(char *name, struct cvars *p, long *offset);

int
sizeof_type(int type);

int
sizeof_struct(int type);

int
sizeof_vars(struct cvars *p);

int
is_pointer(int type);

int
is_char(int type);

int
is_int(int type);

int
is_int_int(int type);

int
is_int_short(int type);

int
is_int_long(int type);

int
is_int_unsigned(int type);

int
is_struct(int type);

int
is_extern(int type);


/*
 1- 3: void, char, int, struct
 4- 5: short, long, or normal int
    6: signed, unsigned
    7: pointer or not
 8-10: extern, volatile, const, auto, staitic, register
17-24: struct index

*/
#define TYPE_MASK 7
#define INTLEN_MASK (3 << 3)
#define INTSIGN_MASK (1 << 5)
#define POINTER_MASK (1 << 6)
#define STORAGE_MASK (7 << 7)

#define TYPE_VOID 1
#define TYPE_CHAR 2
#define TYPE_INT 3
#define TYPE_STRUCT 4

#define INT_SHORT (1 << 3)
#define INT_LONG (2 << 3)

#define INT_UNSIGNED (1 << 5)
#define INT_SIGNED 0

#define TYPE_POINTER (1 << 6)

#define STORAGE_EXTERN (1 << 7)
#define STORAGE_VOLATILE (2 << 7)
#define STORAGE_CONST (3 << 7)
#define STORAGE_AUTO (4 << 7)
#define STORAGE_STATIC (5 << 7)
#define STORAGE_REGISTER (6 << 7)

#define STRUCTIDX_STARTBIT 16


#endif
