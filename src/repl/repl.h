#ifndef __REPL_H__
#define __REPL_H__

#include "../utils/string-utils.h"
#include "../lex/lex.h"
#include "../token/token.h"
#include "../parser/parser.h"
#include "../config.h"
#include "../parser/implementations/bison/lex.yy.h"
#include "../parser/implementations/bison/parser.tab.h"

void print_samples();
void prompt(CompilerOptions options);

#endif /* __REPL_H__ */
