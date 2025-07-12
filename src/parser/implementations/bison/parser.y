
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../errors/syntax-error.h"

void yyerror(const char *s);
int yylex(void);

extern int yytoken;
extern char *yytext;
extern int yylineno;
%}

%union {
    int i;
    float f;
    char* str;
}

%token <i> __INTEGER
%token <f> __FLOAT
%token <str> __IDENTIFIER __STRING

%token __LET __IF __ELSE __ELSEIF __WHILE __FOR
%token __PLUS __MINUS __STAR __SLASH __POW
%token __LPAREN __RPAREN __LBRACE __RBRACE __LBRACK __RBRACK
%token __EQ __ASSIGN __NE __LT __GT __LE __GE
%token __AND __OR __NOT
%token __SEMICOLON __COMMA

%%

program:
    statement_list
    ;

compound_statement:
    __LBRACE statement_list __RBRACE
    ;

statement_list:
    statement statement_list_tail
    ;

statement_list_tail:
    | statement statement_list_tail
    ;

statement:
      expr __SEMICOLON
    | if_statement
    | while_statement
    | for_statement
    ;

expr:
      decl_expr
    | identifier reassign_expr_tail
    ;

reassign_expr_tail:
      __ASSIGN arith_expr
    | arith_expr_tail
    ;

decl_expr:
    __LET identifier decl_expr_assign
    ;

decl_expr_assign:
    | __ASSIGN arith_expr
    ;

arith_expr:
    term arith_expr_tail
    ;

arith_expr_tail:
    arith_expr_tail_rest
    ;

arith_expr_tail_rest:
    | __MINUS term arith_expr_tail_rest
    | __PLUS term arith_expr_tail_rest
    ;

term:
    factor term_list
    ;

term_list:
    term_list_tail
    ;

term_list_tail:
      
    | __SLASH factor term_list_tail
    | __STAR factor term_list_tail
    ;

factor:
    factor_rest
    ;

factor_rest:
      __PLUS pow
    | __MINUS pow
    | pow
    ;

pow:
    atom pow_rest
    ;

pow_rest:
      
    | __POW pow
    ;

atom:
      __INTEGER
    | __FLOAT
    | identifier
    | __STRING
    | __LPAREN arith_expr __RPAREN
    | array
    ;

array:
    __LBRACK items_array __RBRACK
    ;

items_array:
      
    | atom items_array_tail
    ;

items_array_tail:
    | __COMMA atom items_array_tail      
    ;

expr_bool:
    expr_bool_or
    ;

expr_bool_or:
    expr_bool_and expr_bool_or_rest
    ;

expr_bool_or_rest:
    | __OR expr_bool_and expr_bool_or_rest
    ;

expr_bool_and:
    expr_bool_not expr_bool_and_tail
    ;

expr_bool_and_tail:
    | __AND expr_bool_not expr_bool_and_tail
    ;

expr_bool_not:
      __NOT expr_bool_not
    | expr_bool_rel
    ;

expr_bool_rel:
    arith_expr expr_bool_rel_tail
    ;

expr_bool_rel_tail:
    | op_bool arith_expr
    ;

op_bool:
      __LT | __GT | __LE | __GE | __NE | __EQ
    ;

while_statement:
    __WHILE statement_structure
    ;

if_statement:
    __IF statement_structure elseif_statement_list else_statement
    ;

elseif_statement_list:
    elseif_statement_tail
    ;

elseif_statement:
    __ELSEIF statement_structure
    ;

elseif_statement_tail:
    | elseif_statement elseif_statement_tail
    ;

else_statement:
    | __ELSE compound_statement
    ;

statement_structure:
    __LPAREN expr_bool __RPAREN compound_statement
    ;

for_statement:
    __FOR __LPAREN assign_expr_list __SEMICOLON for_expr_bool __SEMICOLON assign_expr_list __RPAREN compound_statement
    ;

for_expr_bool:
    | expr_bool
    ;

assign_expr_list:
    | reassign_expr assign_expr_tail
    ;

assign_expr_tail:
    | __COMMA reassign_expr assign_expr_tail
    ;

reassign_expr:
    identifier __ASSIGN arith_expr
    ;

identifier:
    __IDENTIFIER identifier_array
    ;

identifier_array:
    | __LBRACK arith_expr __RBRACK
    ;

%%

void yyerror(const char *msg) {
    error_handler("Bison Parser Error.");
    fprintf(stderr, "Token atual: '%s' (código: %d)\n", yytext, yychar);
}
