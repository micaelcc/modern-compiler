#include "recursive.h"

void handle_program() {
    //printf("handle program\n");
    while(0 != strcmp(peek_current_token().value, E0F)) {
        handle_statement();
    }
}

void handle_statement() {
        printf("handle  statement %s\n", peek_current_token().value);

    Token curr_tok = peek_current_token();

    if (0 == strcmp(curr_tok.value, IF)) {

    } else if (0 == strcmp(curr_tok.value, WHILE)) {
        handle_while();
    } else if (0 == strcmp(curr_tok.value, FOR)) {
        handle_for();
    } else {
        handle_expr();

        if (peek_current_token().type == TOKEN_SEMICOLON) {
            peek_next_token();
        } else {
            error_handler("Expected: ;");
        }
    }    
}

void handle_while() {
            printf("handle  while %s\n", peek_current_token().value);

    if (0 == strcmp(peek_current_token().value, WHILE)) {
        peek_next_token();
        handle_statement_structure();
    }

    return;
}

void handle_compound_statement() {
            printf("handle_compound_statement %d\n", peek_current_token().type);

    if (peek_current_token().type == TOKEN_LBRACE) {
        if (!look_ahead(TOKEN_RBRACE)) {
            error_handler("Expected: }");
        }

        Token next_tok = peek_next_token();

        while(next_tok.type != TOKEN_RBRACE) {
            handle_statement();
            next_tok = peek_next_token();
        }
    } else {
        error_handler("Expected: {");
    }
}

void handle_expr_bool_rel() {
        printf("handle_expr_bool_rel %s\n", peek_current_token().value);

    Token next_tok = peek_next_token_no_advance();

    if (
        next_tok.type == TOKEN_LE
        || next_tok.type == TOKEN_GE
        || next_tok.type == TOKEN_GT
        || next_tok.type == TOKEN_LT
        || next_tok.type == TOKEN_NOT
        || next_tok.type == TOKEN_NE
    ) {
        handle_atom();

        peek_next_token();

        handle_atom();
    } else {
        handle_arith_expr();
    }
}

void handle_expr_bool_not() {
        printf("handle_expr_bool_not %s\n", peek_current_token().value);

    if (peek_current_token().type == TOKEN_NOT) {
        peek_next_token();
    }

    if (peek_current_token().type == TOKEN_LPAREN) {
        peek_next_token();

        handle_expr_bool();
    } else {
        handle_expr_bool_rel();
    }
}

void handle_expr_bool_and() {
        printf("handle_expr_bool_and %s\n", peek_current_token().value);

    handle_expr_bool_not();

    Token next_tok = peek_current_token();
    while(next_tok.type == TOKEN_AND) {
        peek_next_token();
        
        handle_expr_bool_not();
    }
}

void handle_expr_bool_or() {
        printf("handle_expr_bool_or %s\n", peek_current_token().value);

    handle_expr_bool_and();

    Token next_tok = peek_current_token();
    while(next_tok.type == TOKEN_OR) {
        peek_next_token();

        handle_expr_bool_and();
    }
}

void handle_expr_bool() {
    printf("handle_expr_bool %s\n", peek_current_token().value);

    if (peek_current_token().type == TOKEN_LPAREN) {
        peek_next_token();

        handle_expr_bool_or();

        if (peek_current_token().type == TOKEN_RPAREN) {
            peek_next_token();
        } else {
            error_handler("Expected: )");
        }
    } else {
        handle_expr_bool_or();
    }
}

void handle_statement_structure() {
    printf("handle_statement_structure %s\n", peek_current_token().value);
    if (peek_current_token().type == TOKEN_LPAREN) {
        peek_next_token();

        if (peek_current_token().type == TOKEN_RPAREN) 
            error_handler("Expected: bool expression");
            
        handle_expr_bool();

        if (peek_current_token().type == TOKEN_RPAREN) {
            peek_next_token();
            handle_compound_statement();
        } else {
            error_handler("Expected: )");
        }
    } else {
        error_handler("Expected: ;()");
    }
}

/**/
/**/
/**/

/**/

void handle_for() {
    return;
    /*
    if (0 == strcmp(peek_current_token().value, FOR)) {
        peek_next_token();

        if (peek_current_token().type == TOKEN_LPAREN) {
            peek_next_token();

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                handle_assign_expr_list();

            if (peek_current_token().type == TOKEN_SEMICOLON) {
                peek_next_token();
            } else {
                error_handler("Expected: ; after assign expr list in for");
            }

            if (peek_current_token().type != TOKEN_SEMICOLON)
                handle_expr_bool();

            if (peek_current_token().type == TOKEN_SEMICOLON) {
                peek_next_token();
            } else {
                error_handler("Expected: ; after boolean expression in for");
            }

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                handle_assign_expr_list();

            if (peek_current_token().type == TOKEN_RPAREN) {
                peek_next_token();
            } else {
                error_handler("Expected: ) after for");
            }

            handle_compound_statement();
        } else {
            error_handler("Expected: ( after for key.");
        }
    }
*/
}

void handle_expr() {
    printf("handle expr %s\n", peek_current_token().value);

    Token curr_tok = peek_current_token();
    Token next_tok = peek_next_token_no_advance();

    if (0 == strcmp(curr_tok.value, DEF_VAR)
        || curr_tok.type == TOKEN_ID && (next_tok.type == TOKEN_ASSIGN || next_tok.type == TOKEN_SEMICOLON)
    ) {
        handle_assign_expr();
    } else {
        handle_arith_expr();
    }
}

void handle_assign_expr() {
    printf("handle assign expr %s\n", peek_current_token().value);
    if (0 == strcmp(peek_current_token().value, DEF_VAR)) {
        peek_next_token();

        if (peek_current_token().type != TOKEN_ID) {
            error_handler("Expected: IDENTIFIER after let key");
        }
    }

    if (peek_current_token().type == TOKEN_ID) {
        peek_next_token();

        if (peek_current_token().type == TOKEN_ASSIGN) {
            peek_next_token();

            handle_expr();
        } 
    }
}

void handle_arith_expr() {
    printf("handle arith expr %s\n", peek_current_token().value);
    handle_term();

    Token curr_token = peek_current_token();

    while(curr_token.type == TOKEN_PLUS || curr_token.type == TOKEN_MINUS) {
        curr_token = peek_next_token();

        handle_term();
    }
}


void handle_term() {
    printf("handle term %s\n", peek_current_token().value);
    handle_factor();
    
    Token curr_token = peek_current_token();
    while(curr_token.type == TOKEN_MUL || curr_token.type == TOKEN_DIV) {
        curr_token = peek_next_token();

        handle_factor();
    }
}

void handle_factor() {
    printf("handle factor %s\n", peek_current_token().value);
    Token curr_token = peek_current_token();

    while(curr_token.type == TOKEN_PLUS || curr_token.type == TOKEN_MINUS) {
        //printf("%s %d\n", curr_token.value, curr_token.type);
        curr_token = peek_next_token();
    }

    Token next_tok = peek_next_token_no_advance();

    if (next_tok.type == TOKEN_POW) {
        handle_pow();
    } else {
        handle_atom();
    }
}

void handle_priority_factor() {
    printf("handle priority factor %s\n", peek_current_token().value);
    if (peek_current_token().type == TOKEN_LPAREN) {
        peek_next_token();
        handle_arith_expr();

        if (peek_current_token().type == TOKEN_RPAREN) {
            peek_next_token();
        } else {
            error_handler("Expected: )");
        }
    } else {
        handle_atom();
    }
}

void handle_atom() {
        printf("handle  atom %s\n", peek_current_token().value);

    Token curr_tok = peek_current_token();
    if (curr_tok.type == TOKEN_LPAREN) {
        peek_next_token();

        handle_expr();

        if (peek_current_token().type == TOKEN_RPAREN) {
            peek_next_token();
        } else {
            error_handler("Expected: )");
        }
    } else if(curr_tok.type == TOKEN_INTEGER 
        || curr_tok.type == TOKEN_FLOAT 
        || curr_tok.type == TOKEN_STRING 
        || curr_tok.type == TOKEN_ID) {
        peek_next_token();
    } else {
        error_handler("Expected: INTEGER | FLOAT | ARRAY | IDENTIFIER | STRING | ( EXPRESSION )");
    }
}

void handle_pow() {
                printf("handle  pow %s\n", peek_current_token().value);

    handle_atom();

    if(peek_current_token().type == TOKEN_POW) {
        peek_next_token();

        handle_atom();
    } else {
        error_handler("Expected: ^ pow");
    }
}
