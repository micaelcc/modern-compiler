#ifndef __TABLE_H__
#define __TABLE_H__
#include <stdlib.h>
#include <stdio.h>
#include "../../parse-tree/parse-tree.h"
#include "../../utils/string-utils.h"
#include "../../token/token.h"
#include "../../lex/lex.h"
#include "cjson/cJSON.h"
#define END_OF_TABLE_ITEM -1
typedef enum
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
} NonTerminals ;

typedef int ***ParseTable;

int *search_table_json(NonTerminals, int);
void free_table();
void *load_file();
ParseTable get_table();
void set_table();
int terminal_str_to_enum(const char *);
int non_terminal_str_to_enum(const char *);
char *terminal_enum_to_str(int);
char *non_terminal_enum_to_str(int);
void *load_file();

#endif // __TABLE_H__