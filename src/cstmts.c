#include <stdlib.h>
#include "cexprs.h"
#include "cstmts.h"
#include "misc.h"

struct stmt *new_stmt()
{
	struct stmt *s;

	s = malloc(sizeof(struct stmt));
	if (!s)
		exit_with("Failed alloc memory for new statement\n");

	s->type = 0;
	s->e1 = NULL;
	s->e2 = NULL;
	s->e3 = NULL;
	s->s1 = NULL;
	s->s2 = NULL;

	return s;
}

struct stmt *new_stmt_expr(struct expr *e)
{
	struct stmt *s;

	s = new_stmt();
	s->type = STMT_EXPR;
	s->e1 = e;

	return s;
}

struct stmt *new_stmt_ret(struct expr *e)
{
	struct stmt *s;

	s = new_stmt();
	s->type = STMT_RET;
	s->e1 = e;

	return s;
}

struct stmt *new_stmt_if(struct expr *e, struct stmt *s1, struct stmt *s2)
{
	struct stmt *s;

	s = new_stmt();
	s->type = STMT_IF;
	s->e1 = e;
	s->s1 = s1;
	s->s2 = s2;

	return s;
}

struct stmt *new_stmt_for(struct expr *e1, struct expr *e2, struct expr *e3,
			  struct stmt *s1)
{
	struct stmt *s;

	s = new_stmt();
	s->type = STMT_FOR;
	s->e1 = e1;
	s->e2 = e2;
	s->e3 = e3;
	s->s1 = s1;

	return s;
}
