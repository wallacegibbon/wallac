#ifndef __CSTMTS_H__
#define __CSTMTS_H__

#include "cexprs.h"

#define STMT_EXPR 1
#define STMT_RET 2
#define STMT_IF 3
#define STMT_FOR 4

struct stmt {
	int type;
	struct expr *e1, *e2, *e3;
	struct stmt *s1, *s2;
};

struct stmt *new_stmt_expr(struct expr *e);

struct stmt *new_stmt_ret(struct expr *e);

struct stmt *new_stmt_if(struct expr *e, struct stmt *s1, struct stmt *s2);

struct stmt *new_stmt_for(struct expr *e1, struct expr *e2, struct expr *e3,
			  struct stmt *s);

#endif
