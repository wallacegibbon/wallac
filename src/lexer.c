#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "checkch.h"
#include "limits.h"
#include "vars.h"
#include "token.h"
#include "lexer.h"
#include "hashtbl.h"
#include "misc.h"

#define LEXER_TYPE_STRING 1
#define LEXER_TYPE_FILE 2

struct lexer {
	// char, prev char, line number, character offset
	int ch, pch, line, cursor;
	// string or file
	int type;
	// buffer to hold source content
	char *buff;
	// input buffer, only file lexer use this
	char *input;
	// file name
	char *fname;
	// file descriptor
	int fd;
	// end of line mark
	int eof;
	// pointer to the start of the tokens
	struct token *tk_s;
	// pointer to the current token
	struct token *tk_c;
	// hash table for preprocessor
	struct hashtbl *mtbl;
};

struct token *tokenize_lx(struct lexer *lx);

int assert_ch(struct lexer *lx, char ch, char expected_ch)
{
	if (ch != expected_ch)
		exit_with("%s:%d:[LEXER]Expect char (0x%x), not (0x%x)\n",
			  lx->fname, lx->line, expected_ch, ch);

	return ch;
}

int assert_not_ch(struct lexer *lx, char ch, char unexpected_ch)
{
	if (ch == unexpected_ch)
		exit_with("%s:%d:[LEXER]Unexpected char (0x%x)\n",
			  lx->fname, lx->line, ch);

	return ch;
}

int assert_octal(struct lexer *lx, char ch)
{
	if (!check_decimal(ch))
		exit_with("%s:%d:[LEXER]Expect octal char\n",
			  lx->fname, lx->line);

	return ch;
}

int assert_decimal(struct lexer *lx, char ch)
{
	if (!check_decimal(ch))
		exit_with("%s:%d:[LEXER]Expect decimal char\n",
			  lx->fname, lx->line);

	return ch;
}

int assert_hex(struct lexer *lx, char ch)
{
	if (!check_hex(ch))
		exit_with("%s:%d:[LEXER]Expect hex char\n",
			  lx->fname, lx->line);

	return ch;
}

int assert_ident(struct lexer *lx, char ch)
{
	if (!check_ident(ch))
		exit_with("%s:%d:[LEXER]Expect identifier\n",
			  lx->fname, lx->line);

	return ch;
}

int assert_not_eof(struct lexer *lx, char ch)
{
	if (ch == EOF)
		exit_with("%f:%d:[LEXER]Unexpected EOF\n",
			  lx->fname, lx->line);

	return ch;
}

int lx_getmore_from_file(struct lexer *lx)
{
	int i;

	i = read(lx->fd, lx->input, FILE_BUFF_SIZE);
	if (i < 0)
		exit_with("Failed reading %s(%d)\n", lx->fname, i);

	if (debug)
		printf("Read %d bytes from %s\n", i, lx->fname);

	if (i < FILE_BUFF_SIZE)
		lx->eof = 1;

	*(lx->input + i) = '\0';
	lx->cursor = 0;

	return 1;
}

int file_nextchar(struct lexer *lx)
{
	int ch;

	if (lx->ch == EOF)
		return EOF;

	ch = *(lx->input + lx->cursor);
	if (!ch && !lx->eof)
		lx_getmore_from_file(lx);

	ch = *(lx->input + lx->cursor);
	if (!ch)
		ch = EOF;

	lx->pch = lx->ch;
	lx->ch = ch;
	lx->cursor++;

	if (ch == '\n')
		lx->line++;

	if (lx->line == INT_MAX)
		exit_with("File %s too long(more than %d lines)\n",
			  lx->fname, INT_MAX);

	return ch;
}

int str_nextchar(struct lexer *lx)
{
	int ch;

	if (lx->ch == EOF)
		return EOF;

	ch = *(lx->input + lx->cursor);
	if (!ch)
		ch = EOF;

	lx->pch = lx->ch;
	lx->ch = ch;
	lx->cursor++;

	return ch;
}

int nextchar(struct lexer *lx)
{
	int ch;

	switch (lx->type) {
	case LEXER_TYPE_FILE:
		ch = file_nextchar(lx);
		break;
	case LEXER_TYPE_STRING:
		ch = str_nextchar(lx);
		break;
	default:
		exit_with("nextchar, Invalid lexer type: %d\n", lx->type);
	}
	//printf("\neating character: %c\n", ch);
	return ch;
}

int nextchar_noteof(struct lexer *lx)
{
	int ch;

	ch = nextchar(lx);
	assert_not_eof(lx, ch);
	return ch;
}

struct lexer *new_lexer_common(char *buff, struct hashtbl *mtbl)
{
	struct lexer *lx;

	lx = malloc(sizeof(struct lexer));
	if (!lx)
		exit_with("Failed alloc memory for lexer\n");

	lx->type = 0;

	lx->fname = NULL;
	lx->fd = 0;
	lx->eof = 0;

	lx->input = NULL;
	lx->buff = buff;

	lx->mtbl = mtbl;

	lx->line = 1;
	lx->ch = '\0';
	lx->pch = '\0';
	lx->cursor = 0;

	lx->tk_s = NULL;
	lx->tk_c = NULL;

	return lx;
}

struct lexer *new_lexer_file(char *fname, char *buff, struct hashtbl *mtbl)
{
	struct lexer *lx;
	char *fbuff;
	int i;

	lx = new_lexer_common(buff, mtbl);
	lx->fname = fname;
	lx->type = LEXER_TYPE_FILE;

	i = open(fname, O_RDONLY);
	if (i < 0)
		exit_with("Failed opening %s(%d)\n", fname, i);
	if (i < 3)
		exit_with("Invalid fd(%d) for %s\n", i, fname);

	lx->fd = i;
	// create file buffer to hold file contents
	fbuff = malloc(FILE_BUFF_SIZE + 1);
	if (!fbuff)
		exit_with("Failed alloc buffer for %s\n", fname);

	lx->input = fbuff;
	// fill the file content buffer
	lx_getmore_from_file(lx);

	nextchar(lx);
	return lx;
}

struct lexer *new_lexer_str(char *str, char *buff, struct hashtbl *mtbl)
{
	struct lexer *lx;
	lx = new_lexer_common(buff, mtbl);
	lx->type = LEXER_TYPE_STRING;
	lx->input = str;

	nextchar(lx);
	return lx;
}

void free_lexer_file(struct lexer *lx)
{
	int i;
	i = close(lx->fd);
	if (i == -1)
		exit_with("close lx->fd(%d) error: %d\n", lx->fd, errno);
	free(lx->input);
	free(lx);
}

void free_lexer_str(struct lexer *lx)
{
	free(lx);
}

void free_lexer(struct lexer *lx)
{
	switch (lx->type) {
	case LEXER_TYPE_FILE:
		free_lexer_file(lx);
		break;
	case LEXER_TYPE_STRING:
		free_lexer_str(lx);
		break;
	default:
		exit_with("free_lexer, Invalid lexer type: %d\n", lx->type);
	}
}

void join_token(struct lexer *lx, struct token *t)
{
	if (lx->tk_c) {
		t->prev = lx->tk_c;
		lx->tk_c->next = t;
		lx->tk_c = t;
	} else {
		t->prev = NULL;
		lx->tk_c = t;
		lx->tk_s = t;
	}
}

void join_new_token(struct lexer *lx, int line, int type, void *p)
{
	struct token *t;

	t = new_token(type, p);
	if (!t)
		exit_with("failed creating token, memory not enough\n");

	t->fname = lx->fname;
	t->line = line;

	join_token(lx, t);
}

int join_new_token_forward(struct lexer *lx, int line, int type)
{
	join_new_token(lx, line, type, NULL);
	nextchar(lx);

	return 1;
}

int fix_position(struct lexer *lx, struct token *t, int line)
{
	t->fname = lx->fname;
	t->line = line;

	return 1;
}

int join_chain(struct lexer *lx, int line, struct token *orig, int fixpos)
{
	struct token *t, *p;

	if (!orig)
		return 1;

	t = copy_token_chain(orig);
	if (!t)
		return 1;

	join_token(lx, t);

	for (; t; p = t, t = t->next) {
		if (fixpos)
			fix_position(lx, t, line);
	}
	lx->tk_c = p;
	return 1;
}

int get_ellipsis(struct lexer *lx, int line)
{
	int ch;

	ch = nextchar_noteof(lx);
	if (ch != '.')
		exit_with("%s:%d:[LEXER]Unsupported element \"..\"\n",
			  lx->fname, line);

	join_new_token(lx, line, TK_ELLIPSIS, NULL);
	nextchar(lx);
	return 1;
}

int get_dot_ellipsis(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '.')
		return get_ellipsis(lx, line);

	if (check_decimal(ch))
		exit_with("%s:%d:[LEXER]Float literal is not supported\n",
			  lx->fname, line);

	join_new_token(lx, line, TK_DOT, NULL);

	return 1;
}

int get_minus_dminus_pointsto(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '-')
		return join_new_token_forward(lx, line, TK_DMINUS);
	if (ch == '>')
		return join_new_token_forward(lx, line, TK_POINTSTO);

	join_new_token(lx, line, TK_MINUS, NULL);

	return 1;
}

int get_plus_dplus(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '+')
		return join_new_token_forward(lx, line, TK_DPLUS);

	join_new_token(lx, line, TK_PLUS, NULL);

	return 1;
}

int get_and_dand(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '&')
		return join_new_token_forward(lx, line, TK_DAND);

	join_new_token(lx, line, TK_AND, NULL);

	return 1;
}

int get_or_dor(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '|')
		return join_new_token_forward(lx, line, TK_DOR);

	join_new_token(lx, line, TK_DOR, NULL);

	return 1;
}

int get_assign_eq(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '=')
		return join_new_token_forward(lx, line, TK_EQ);

	join_new_token(lx, line, TK_ASSIGN, NULL);

	return 1;
}

int get_gt_geq_shr(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '=')
		return join_new_token_forward(lx, line, TK_GEQ);
	if (ch == '>')
		return join_new_token_forward(lx, line, TK_SHR);

	join_new_token(lx, line, TK_GT, NULL);

	return 1;
}

int get_lt_leq_shl(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '=')
		return join_new_token_forward(lx, line, TK_LEQ);
	if (ch == '<')
		return join_new_token_forward(lx, line, TK_SHL);

	join_new_token(lx, line, TK_LT, NULL);

	return 1;
}

int get_exclamation_neq(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '=')
		return join_new_token_forward(lx, line, TK_NEQ);

	join_new_token(lx, line, TK_EXCLAMATION, NULL);

	return 1;
}

int jump_multi_comments_recur(struct lexer *lx)
{
	int ch;

	for (; ch = lx->ch, ch != '*';)
		nextchar_noteof(lx);

	ch = nextchar_noteof(lx);
	if (ch != '/')
		return jump_multi_comments_recur(lx);
	else
		return 1;
}

int jump_multi_comments(struct lexer *lx)
{
	nextchar_noteof(lx);
	jump_multi_comments_recur(lx);
	nextchar(lx);

	return 1;
}

int skip_line(struct lexer *lx)
{
	int ch;

	for (; ch = lx->ch, ch != EOF && ch != '\n';)
		nextchar(lx);

	nextchar(lx);

	return 1;
}

int get_divide_or_jump_comments(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);

	if (ch == '*')
		return jump_multi_comments(lx);
	if (ch == '/')
		return skip_line(lx);

	join_new_token(lx, line, TK_DIVIDE, NULL);

	return 1;
}

int check_digit(char ch, int base)
{
	if (base == 16)
		return check_hex(ch);
	if (base == 10)
		return check_decimal(ch);
	if (base == 8)
		return check_octal(ch);

	return 0;
}

int collect_digit_chars(struct lexer *lx, int line, int base)
{
	char *buffer;
	int cnt;

	buffer = lx->buff;
	*buffer++ = lx->ch;

	cnt = 1;
	for (; cnt < BUFF_SIZE && check_digit(nextchar(lx), base); cnt++)
		*buffer++ = lx->ch;

	*buffer = '\0';

	return cnt;
}

int cnv_digit(int ch)
{
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	exit_with("invalid digit char: %c\n", ch);
}

int check_overflow(char *numstr, int base)
{
	char *maxstr;
	int n, m;

	switch (base) {
	case 16:
		maxstr = MAX_HEX_STRING;
		break;
	case 10:
		maxstr = MAX_DECIMAL_STRING;
		break;
	case 8:
		maxstr = MAX_OCTAL_STRING;
		break;
	default:
		exit_with("unknown base %d\n", base);
	}

	n = strlen(numstr);
	m = strlen(maxstr);
	if (n > m || (n == m && strcmp(numstr, maxstr) > 0))
		return 1;
	else
		return 0;
}

void get_integer_num(struct lexer *lx, int line, int base)
{
	char *buffer;
	long i;

	collect_digit_chars(lx, line, base);
	buffer = lx->buff;
	if (check_overflow(buffer, base))
		exit_with("%s:%d:[LEXER]Integer literal too big\n",
			  lx->fname, line);

	for (i = 0; *buffer; buffer++)
		i = i * base + cnv_digit(*buffer);

	join_new_token(lx, line, TK_CINT, (void *)i);
}

int get_integer(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = lx->ch;
	// missing leading 0, normal decimal
	if (ch != '0') {
		get_integer_num(lx, line, 10);
		return 1;
	}

	ch = nextchar_noteof(lx);
	// have to be number like 0x1, 01, or 0.
	if (ch == 'x' || ch == 'X') {
		nextchar_noteof(lx);
		get_integer_num(lx, line, 16);
	} else if (check_octal(ch)) {
		get_integer_num(lx, line, 8);
	} else {
		join_new_token(lx, line, TK_CINT, (void *)0);
	}
	return 1;
}

int get_identifier(struct lexer *lx)
{
	struct token *mtk;
	char *buffer;
	int line, cnt, kw;

	line = lx->line;
	buffer = lx->buff;
	*buffer++ = lx->ch;

	// collect the identifier into the buffer
	for (cnt = 1; cnt < BUFF_SIZE && check_ident(nextchar(lx)); cnt++)
		*buffer++ = lx->ch;
	*buffer = '\0';

	if (cnt > MAX_IDENT_LENGTH)
		exit_with("%s:%d:[LEXER]Identifier too long\n",
			  lx->fname, line);

	buffer = lx->buff;

	kw = try_get_keyword(buffer);
	if (kw) {
		join_new_token(lx, line, kw, NULL);
		return 1;
	}
	// TODO: the whole macro system should be rewritten
	mtk = hashtbl_get(lx->mtbl, buffer);
	if (mtk)
		return join_chain(lx, line, mtk, 1);

	join_new_token(lx, line, TK_IDENT, copy_string(buffer));
	return 1;
}

int get_hex_escape(struct lexer *lx)
{
	int r, t;

	t = nextchar_noteof(lx);
	assert_hex(lx, t);
	r = cnv_digit(t) * 16;

	t = nextchar_noteof(lx);
	assert_hex(lx, t);
	r += cnv_digit(t);

	nextchar_noteof(lx);

	return r;
}

int get_octal_escape(struct lexer *lx)
{
	int ch, v;
	v = cnv_digit(lx->ch);

	ch = nextchar_noteof(lx);

	if (!check_octal(ch))
		return v;
	v *= 8;
	v += cnv_digit(ch);

	ch = nextchar_noteof(lx);

	if (!check_octal(ch))
		return v;
	v *= 8;
	v += cnv_digit(ch);

	nextchar_noteof(lx);

	return v;
}

// ascii code 1 will not be valid character in a file, use it as a sign of
// '\n' after '\'
#define ESCAPE_NEWLINE 1
char escape_src[8] = { 'a', 'b', 't', 'n', 'v', 'f', 'r', '\n' };
char escape_dst[8] = { 7, 8, 9, 10, 11, 12, 13, ESCAPE_NEWLINE };

int get_normal_escape(struct lexer *lx)
{
	int i, ch = -1;

	for (i = 0; i < 8; i++) {
		if (escape_src[i] == lx->ch)
			ch = escape_dst[i];
	}
	// characters are what they were
	if (ch == -1)
		ch = lx->ch;

	nextchar_noteof(lx);
	return ch;
}

int get_escape_seq(struct lexer *lx)
{
	int ch, ret;

	ch = nextchar_noteof(lx);
	if (check_octal(ch)) {
		ret = get_octal_escape(lx);
	} else if (ch == 'x') {
		ret = get_hex_escape(lx);
	} else {
		ret = get_normal_escape(lx);
	}

	return ret;
}

int get_character(struct lexer *lx)
{
	int line, ch;

	line = lx->line;
	ch = nextchar_noteof(lx);
	if (ch == '\'')
		exit_with("%s:%d:[LEXER]Empty character literal\n",
			  lx->fname, line);
	if (ch == '\n')
		exit_with("%s:%d:[LEXER]Missing terminating \"'\"\n",
			  lx->fname, line);

	if (ch == '\\')
		ch = get_escape_seq(lx);
	else
		nextchar_noteof(lx);

	if (ch == ESCAPE_NEWLINE)
		exit_with("%s:%d:[LEXER]escape new line is now allowed\n",
			  lx->fname, line);

	assert_ch(lx, lx->ch, '\'');
	join_new_token(lx, line, TK_CCHAR, (void *)ch);
	nextchar(lx);
	return 1;
}

int get_strchar_sub(struct lexer *lx)
{
	char ch;

	ch = lx->ch;
	if (ch == '\n')
		exit_with("%s:%d:[LEXER]Missing terminating '\"'\n",
			  lx->fname, lx->line - 1);
	if (ch == '"')
		return 0;
	if (ch == '\\')
		ch = get_escape_seq(lx);
	else
		nextchar_noteof(lx);

	return ch;
}

// returns 0 when string is finished
int get_strchar(struct lexer *lx, char *c)
{
	char ch;

	do {
		ch = get_strchar_sub(lx);
	} while (ch == ESCAPE_NEWLINE);

	*c = ch;
	if (ch == 0)
		return 0;
	else
		return 1;
}

int get_string(struct lexer *lx)
{
	char *buffer, ch;
	int line, cnt;

	line = lx->line;
	buffer = lx->buff;
	ch = nextchar_noteof(lx);

	for (cnt = 0; cnt < BUFF_SIZE && get_strchar(lx, &ch); cnt++)
		*buffer++ = ch;
	*buffer = '\0';

	if (cnt == MAX_CSTR_LENGTH)
		exit_with("%s:%d:[LEXER]String too long\n", lx->fname, line);

	join_new_token(lx, line, TK_CSTR, copy_string(lx->buff));
	nextchar(lx);
	return 1;
}

char *header_filename(struct lexer *lx, int line, char *s)
{
	char ch, *a, *b;

	a = s;
	for (; ch = *a, ch && check_space(ch);)
		a++;

	if (!ch || ch != '"')
		exit_with("%s:%d:[LEXER]Only support #include \"...\"\n",
			  lx->fname, line);

	a++;
	b = s;
	for (; ch = *a, check_ident(ch) || ch == '.' || ch == '/';)
		*b++ = *a++;

	*b = '\0';

	if (*a != '"')
		exit_with("%s:%d:[LEXER]Invalid include directive\n",
			  lx->fname, line);

	a++;
	for (; *a && check_space(*a);)
		a++;

	if (*a)
		exit_with("%s:%d:[LEXER]Invalid content after include\n",
			  lx->fname, line);

	return s;
}

int handle_header(struct lexer *lx, int line, char *s)
{
	struct token *tks;
	struct lexer *ilx;
	char *incname, *pname;
	int i;

	incname = header_filename(lx, line, s + 7);
	pname = mkpath_from(lx->fname, incname);

	if (debug)
		printf("<%s> is including <%s>...\n", lx->fname, pname);

	ilx = new_lexer_file(pname, lx->buff, lx->mtbl);
	tks = tokenize_lx(ilx);

	free_lexer(ilx);

	if (tks)
		join_chain(lx, line, tks, 0);

	return 1;
}

char *shift_macroname(struct lexer *lx, int line, char *s, int offset)
{
	char *a, *b, *r;
	int i;

	a = s + offset;
	if (!check_space(*a))
		exit_with("%s:%d:[LEXER]Invalid define directive\n",
			  lx->fname, line);

	for (; *a && check_space(*a);)
		a++;

	if (!check_ident_start(*a))
		exit_with("%s:%d:[LEXER]Invalid macro name\n", lx->fname, line);

	b = a;
	for (i = 0; check_ident(*a); a++)
		i++;

	r = malloc(i);
	if (!r)
		exit_with("%s:%d:[LEXER]Failed alloc memory for macro name\n",
			  lx->fname, line);

	strncpy(r, b, i);
	strcpy(s, a);

	return r;
}

int handle_define(struct lexer *lx, int line, char *s)
{
	struct token *tks;
	struct lexer *slx;
	char *name;
	int i;

	name = shift_macroname(lx, line, s, 6);

	if (try_get_keyword(name))
		exit_with("%s:%d:[LEXER]keywords can not be macro name\n",
			  lx->fname, line);

	slx = new_lexer_str(s, lx->buff, lx->mtbl);
	slx->line = line;
	slx->fname = lx->fname;

	tks = tokenize_lx(slx);

	free_lexer(slx);

	i = hashtbl_add(lx->mtbl, name, (void *)tks);
	if (!i)
		exit_with("%s:%d[LEXER]macro %s is already defined\n",
			  lx->fname, line, name);

	return 1;
}

int skip_until_endif(struct lexer *lx)
{
	char ch, *buffer;
	int i;

	for (; ch = lx->ch, ch != '#';)
		skip_line(lx);

	buffer = lx->buff;

	for (i = 0; i < 5; i++)
		*buffer++ = nextchar_noteof(lx);

	*buffer = '\0';

	if (strcmp("endif", lx->buff))
		return skip_until_endif(lx);
	else
		return skip_line(lx);
}

int handle_ifndef(struct lexer *lx, int line, char *s)
{
	char *name;

	name = shift_macroname(lx, line, s, 6);

	if (hashtbl_keyexist(lx->mtbl, name))
		return skip_until_endif(lx);
	else
		return 1;
}

char *preprocess_content(struct lexer *lx)
{
	char ch, *buffer;
	int line, cnt;

	buffer = lx->buff;
	line = lx->line;

	if ((lx->line == 1 && lx->pch != 0)
	    || (lx->line != 1 && lx->pch != '\n'))
		exit_with("%s:%d:[LEXER]\"#\" should be the 1st ch of a line\n",
			  lx->fname, line);

	ch = nextchar_noteof(lx);

	if (check_space(ch))
		exit_with("%s:%d:[LEXER]Do not write space after \"#\"\n",
			  lx->fname, line);

	for (cnt = 0; ch != '\n' && ch != EOF && cnt < BUFF_SIZE;
	     ch = nextchar(lx), cnt++)
		*buffer++ = ch;

	*buffer = '\0';

	if (cnt == BUFF_SIZE)
		exit_with("%s:%d:[LEXER]line too long\n", lx->fname, line);

	if (*(buffer - 1) == '\\')
		exit_with("%s:%d:[LEXER]Multiline in preprocess is invalid\n",
			  lx->fname, line);

	nextchar(lx);

	return lx->buff;
}

int preprocess(struct lexer *lx)
{
	char *buffer;
	int line;

	line = lx->line;
	buffer = preprocess_content(lx);

	if (!strncmp(buffer, "include", 7))
		return handle_header(lx, line, buffer);

	if (!strncmp(buffer, "define", 6))
		return handle_define(lx, line, buffer);

	if (!strncmp(buffer, "ifndef", 6))
		return handle_ifndef(lx, line, buffer);

	if (!strncmp(buffer, "endif", 5))
		return 1;

	exit_with("%s:%d:[LEXER]Unknown preprocess directive %s\n",
		  lx->fname, line, buffer);

	return 1;
}

int get_token(struct lexer *lx)
{
	for (; check_space(lx->ch);)
		nextchar(lx);

	switch (lx->ch) {
	case EOF:
		return 0;
	case ';':
		return join_new_token_forward(lx, lx->line, TK_SEMICOLON);
	case ':':
		return join_new_token_forward(lx, lx->line, TK_COLON);
	case ',':
		return join_new_token_forward(lx, lx->line, TK_COMMA);
	case '?':
		return join_new_token_forward(lx, lx->line, TK_QUESTION);
	case '^':
		return join_new_token_forward(lx, lx->line, TK_CARET);
	case '~':
		return join_new_token_forward(lx, lx->line, TK_TILDE);
	case '*':
		return join_new_token_forward(lx, lx->line, TK_ASTERISK);
	case '%':
		return join_new_token_forward(lx, lx->line, TK_MOD);
	case '{':
		return join_new_token_forward(lx, lx->line, TK_BEGIN);
	case '}':
		return join_new_token_forward(lx, lx->line, TK_END);
	case '[':
		return join_new_token_forward(lx, lx->line, TK_OPENBR);
	case ']':
		return join_new_token_forward(lx, lx->line, TK_CLOSEBR);
	case '(':
		return join_new_token_forward(lx, lx->line, TK_OPENPA);
	case ')':
		return join_new_token_forward(lx, lx->line, TK_CLOSEPA);
	case '/':
		return get_divide_or_jump_comments(lx);
	case '!':
		return get_exclamation_neq(lx);
	case '+':
		return get_plus_dplus(lx);
	case '-':
		return get_minus_dminus_pointsto(lx);
	case '.':
		return get_dot_ellipsis(lx);
	case '&':
		return get_and_dand(lx);
	case '|':
		return get_or_dor(lx);
	case '=':
		return get_assign_eq(lx);
	case '>':
		return get_gt_geq_shr(lx);
	case '<':
		return get_lt_leq_shl(lx);
	case '\'':
		return get_character(lx);
	case '"':
		return get_string(lx);
	case '#':
		return preprocess(lx);
	}
	if (check_ident_start(lx->ch))
		return get_identifier(lx);
	if (check_decimal(lx->ch))
		return get_integer(lx);

	exit_with("%s:%d:[LEXER]Unknown char (0x%x)\n",
		  lx->fname, lx->line, lx->ch);
	return 1;
}

struct token *tokenize_lx(struct lexer *lx)
{
	for (; get_token(lx);) ;

	return lx->tk_s;
}

struct token *tokenize(char *filename)
{
	struct hashtbl *macrotbl;
	struct token *tks;
	struct lexer *lx;
	char *buff;

	buff = malloc(BUFF_SIZE + 1);
	if (!buff)
		exit_with("Failed alloc memory for lexer buffer\n");

	macrotbl = new_hashtbl(40);

	lx = new_lexer_file(filename, buff, macrotbl);
	tks = tokenize_lx(lx);

	free_lexer(lx);
	free(buff);

	//hashtbl_print(macrotbl);

	return tks;
}
