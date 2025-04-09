#include <stdlib.h>
#include <stdio.h>
#include "../../ast/ast.h"
#include "../../utils/string-utils.h"
#include "../../token/token.h"
#include "../../lex/lex.h"
#include "cjson/cJSON.h"
#define END_OF_TABLE_ITEM -1
enum NonTerminals
{
    NonTermProgram = 0,
    NonTermCompoundStatement,
    NonTermStatementList,
    NonTermStatementListTail,
    NonTermStatement,
    NonTermExpr,
    NonTermReassignExpr,
    NonTermDeclExpr,
    NonTermDeclExprAssign,
    NonTermReassignExprTail,
    NonTermArithExpr,
    NonTermArithExprTail,
    NonTermArithExprTailRest,
    NonTermTerm,
    NonTermTermList,
    NonTermTermListTail,
    NonTermFactor,
    NonTermFactorRest,
    NonTermPow,
    NonTermPowRest,
    NonTermAtom,
    NonTermArray,
    NonTermItemsArray,
    NonTermItemsArrayTail,
    NonTermExprBool,
    NonTermExprBoolOr,
    NonTermExprBoolOrRest,
    NonTermExprBoolAnd,
    NonTermExprBoolAndTail,
    NonTermExprBoolNot,
    NonTermExprBoolRel,
    NonTermExprBoolRelTail,
    NonTermExprBoolRelFactor,
    NonTermOpBool,
    NonTermWhileStatement,
    NonTermIfStatement,
    NonTermElseifStatementList,
    NonTermElseifStatement,
    NonTermElseifStatementTail,
    NonTermElseStatement,
    NonTermStatementStructure,
    NonTermForStatement,
    NonTermForExprBool,
    NonTermAssignExprList,
    NonTermAssignExprTail,
    NonTermIdentifier,
    NonTermIdentifierArray,
};

typedef int ***ParseTable;

void *load_file();
ParseTable get_table();
void set_table();
int terminal_str_to_enum(const char *);
int non_terminal_str_to_enum(const char *);
char *terminal_enum_to_str(int);
char *non_terminal_enum_to_str(int);
void *load_file();
int *search_table_json(enum NonTerminals, int);