#ifndef __NON_RECURSIVE_H__
#define __NON_RECURSIVE_H__
#include <stdbool.h>
#include "../errors/syntax-error.h"
#include "../table/table.h"
#include "../../parse-tree/parse-tree.h"
#include "../../utils/string-utils.h"
#include "../../token/token.h"
#include "../../lex/lex.h"
#include "../../stack/stack.h"

ParseTree *table_driven_parser();
void table_driven_parser_only_syntax_check();

#endif // __NON_RECURSIVE_H__