#ifndef __CSTMTS_H__
#define __CSTMTS_H__

#include "cexprs.h"

#define EXPR_STMT 1
#define RET_STMT 2
#define IF_STMT 3
#define FOR_STMT 4

struct stmt { int type; struct expr *e1, *e2, *e3; struct stmt *s1, *s2; };

#endif
