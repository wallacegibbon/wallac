#ifndef __PARSER_H__
#define __PARSER_H__

#include "token.h"

struct ast {
	struct linktbl *sdefs, *gvars, *funcs;
};

struct ast *parse(struct token *tks);

#endif
