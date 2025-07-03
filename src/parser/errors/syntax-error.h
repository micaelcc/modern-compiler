#ifndef __SYNTAX_ERROR_H__
#define __SYNTAX_ERROR_H__
#include "../table/table.h"
#include "../../token/token.h"
#include "../../lex/lex.h"

void error_handler(char[]);
void err_expected(char *, char *);
void err_expected_term(ParseTable, int);

#endif // __SYNTAX_ERROR_H__