#include "../../lex/lex.h"
#include "../../token/token.h"
#include "../errors/syntax-error.h"

extern int PARSER_ERROR;

void handle_program();
void handle_statement();
void handle_expr();
void handle_assign_expr();
void handle_arith_expr();
void handle_assign_expr_list();
void handle_expr_bool_not();
void handle_expr_bool();
void handle_statement_structure();
void handle_for();
void handle_while();
void handle_term();
void handle_factor();
void handle_atom();
void handle_pow();
void handle_if();
void handle_compound_statement();
void handle_expr_bool_op();
void handle_expr_bool_rel();
void handle_expr_bool_and();
void handle_expr_bool_or();
void handle_assign_expr_tail();
void handle_items_array();
void handle_array();
void handle_array_atom();
