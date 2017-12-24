#ifndef __TYPE_H__
#define __TYPE_H__



#include "cvars.h"

struct cstruct { char *name; struct cvars *vars; struct cstruct *next; };


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
#define VAR_MASK (7 << 7)

#define TYPE_VOID 1
#define TYPE_CHAR 2
#define TYPE_INT 3
#define TYPE_STRUCT 4

#define INT_SHORT (1 << 3)
#define INT_LONG (2 << 3)

#define INT_UNSIGNED (1 << 5)
#define INT_SIGNED 0

#define TYPE_POINTER (1 << 6)

#define VAR_EXTERN (1 << 7)
#define VAR_VOLATILE (2 << 7)
#define VAR_CONST (3 << 7)
#define VAR_AUTO (4 << 7)
#define VAR_STATIC (5 << 7)
#define VAR_REGISTER (6 << 7)


#define STRUCTIDX_STARTBIT 16



#define IS_CHAR(v) (((v) & TYPE_MASK) === TYPE_CHAR)

#define IS_INT(v) (((v) & TYPE_MASK) === TYPE_INT)
#define IS_SHORT(v) (((v) & INTLEN_MASK) == INT_SHORT)
#define IS_LONG(v) (((v) & INTLEN_MASK) == INT_LONG)
#define IS_UNSIGNED(v) (((v) & INTSIGN_MASK) == INT_UNSIGNED)


#define IS_EXTERN(v) ((v) & VAR_EXTERN)
#define IS_VOLATILE(v) ((v) & VAR_VOLATILE)
#define GET_STRUCTIDX(v) (((v) >> STRUCTIDX_STARTBIT) & 0xff)



#endif

