#include "../../lex/lex.h"
#include "../../token/token.h"
#include "../errors/syntax-error.h"

void handle_program();

void handle_statement();

void handle_expr();

void handle_assign_expr();

void handle_arith_expr();


void handle_term();

void handle_factor();

void handle_priority_factor();

void handle_atom();

void handle_pow();
