#include "recursive.h"

void handle_program()
{
    if (PARSER_ERROR) return;
    while (!PARSER_ERROR && 0 != strcmp(peek_current_token().value, E0F))
    {
        handle_statement();
    }
}

void handle_else()
{
    if (PARSER_ERROR) return;
    if (0 == strcmp(peek_current_token().value, ELSE))
    {
        peek_next_token();
        handle_compound_statement();
    }
}

void handle_if()
{
    if (PARSER_ERROR) return;
    if (0 == strcmp(peek_current_token().value, IF))
    {
        peek_next_token();
        handle_statement_structure();

        while (!PARSER_ERROR && 0 == strcmp(peek_current_token().value, ELSE))
        {
            if (0 == strcmp(peek_next_token_no_advance().value, IF))
            {
                peek_next_token();

                handle_if();
            }
            else
            {
                handle_else();
            }
        }
    }
}

void handle_statement()
{
    if (PARSER_ERROR) return;
    Token curr_tok = peek_current_token();
    if (0 == strcmp(curr_tok.value, IF))
    {
        handle_if();
    }
    else if (0 == strcmp(curr_tok.value, WHILE))
    {
        handle_while();
    }
    else if (0 == strcmp(curr_tok.value, FOR))
    {
        handle_for();
    }
    else if (curr_tok.type == TOKEN_AND 
        || curr_tok.type == TOKEN_ASSIGN 
        || curr_tok.type == TOKEN_COMMA 
        || curr_tok.type == TOKEN_DIV 
        || curr_tok.type == TOKEN_EQ 
        || curr_tok.type == TOKEN_GE 
        || curr_tok.type == TOKEN_GT 
        || curr_tok.type == TOKEN_LBRACE 
        || curr_tok.type == TOKEN_LE 
        || curr_tok.type == TOKEN_LSQUARE 
        || curr_tok.type == TOKEN_LT 
        || curr_tok.type == TOKEN_MUL 
        || curr_tok.type == TOKEN_NE 
        || curr_tok.type == TOKEN_NOT 
        || curr_tok.type == TOKEN_OR 
        || curr_tok.type == TOKEN_RSQUARE 
        || curr_tok.type == TOKEN_RPAREN 
        || curr_tok.type == TOKEN_RBRACE 
        || curr_tok.type == TOKEN_POW
    )
    {
        error_handler("Expected: Expr | If | While | For");
    }
    else
    {
        handle_expr();

        if (peek_current_token().type == TOKEN_SEMICOLON)
        {
            peek_next_token();
        }
        else
        {
            error_handler("Expected: ;");
        }
    }
}

void handle_while()
{
    if (PARSER_ERROR) return;
    if (0 == strcmp(peek_current_token().value, WHILE))
    {
        peek_next_token();
        handle_statement_structure();
    }

    return;
}

void handle_compound_statement()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_LBRACE)
    {
        if (!look_ahead(TOKEN_RBRACE, true, true))
        {
            error_handler("Expected: }");
        }

        peek_next_token();

        while (!PARSER_ERROR && peek_current_token().type != TOKEN_RBRACE)
        {

            handle_statement();
        }

        if (peek_current_token().type == TOKEN_RBRACE)
        {
            peek_next_token();
        }
    }
    else
    {
        error_handler("Expected: {");
    }
}

void handle_expr_bool_op()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_EQ 
        || peek_current_token().type == TOKEN_LE 
        || peek_current_token().type == TOKEN_GE 
        || peek_current_token().type == TOKEN_NE
    )
    {
        peek_next_token();
        peek_next_token();
    }
    else if (peek_current_token().type == TOKEN_GT 
        || peek_current_token().type == TOKEN_LT 
        || peek_current_token().type == TOKEN_NOT
    )
    {
        peek_next_token();
    }
}
void handle_expr_bool_rel()
{
    if (PARSER_ERROR) return;
    Token next_tok = peek_next_token_no_advance();

    if (
        look_ahead(TOKEN_LE, false, true) 
        || look_ahead(TOKEN_GE, false, true) 
        || look_ahead(TOKEN_GT, false, true) 
        || look_ahead(TOKEN_LT, false, true) 
        || look_ahead(TOKEN_NOT, false, true) 
        || look_ahead(TOKEN_NE, false, true) 
        || look_ahead(TOKEN_EQ, false, true)
    )
    {
        handle_arith_expr();

        peek_next_token();

        handle_arith_expr();
    }
    else
    {
        handle_arith_expr();
    }
}

void handle_expr_bool_not()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_NOT)
    {
        peek_next_token();
    }

    if (peek_current_token().type == TOKEN_LPAREN)
    {
        peek_next_token();

        handle_expr_bool();

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            peek_next_token();
        }
        else
        {
            error_handler("Expected: )");
        }
    }
    else
    {
        handle_expr_bool_rel();
    }
}

void handle_expr_bool_and()
{
    if (PARSER_ERROR) return;
    handle_expr_bool_not();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_AND)
    {
        peek_next_token();

        handle_expr_bool_not();
    }
}

void handle_expr_bool_or()
{
    if (PARSER_ERROR) return;
    handle_expr_bool_and();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_OR)
    {
        peek_next_token();

        handle_expr_bool_and();
    }
}

void handle_expr_bool()
{
    if (PARSER_ERROR) return;
    handle_expr_bool_or();
}

void handle_statement_structure()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_LPAREN)
    {
        peek_next_token();

        if (peek_current_token().type == TOKEN_RPAREN)
            error_handler("Expected: bool expression");

        handle_expr_bool();

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            peek_next_token();
            handle_compound_statement();
        }
        else
        {
            error_handler("Expected: )");
        }
    }
    else
    {
        error_handler("Expected: ;()");
    }
}

void handle_assign_expr_tail()
{
    if (PARSER_ERROR) return;
    handle_assign_expr();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_COMMA)
    {
        peek_next_token();

        handle_assign_expr();
    }
}

void handle_assign_expr_list()
{
    if (PARSER_ERROR) return;
    handle_assign_expr();
    if (peek_current_token().type == TOKEN_COMMA)
    {
        peek_next_token();

        handle_assign_expr_tail();
    }
}

void handle_for()
{
    if (PARSER_ERROR) return;
    if (0 == strcmp(peek_current_token().value, FOR))
    {
        peek_next_token();

        if (peek_current_token().type == TOKEN_LPAREN)
        {
            peek_next_token();

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                handle_assign_expr_list();

            if (peek_current_token().type == TOKEN_SEMICOLON)
            {
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ; after assign expr list in for");
            }

            if (peek_current_token().type != TOKEN_SEMICOLON)
                handle_expr_bool();

            if (peek_current_token().type == TOKEN_SEMICOLON)
            {
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ; after boolean expression in for");
            }

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                handle_assign_expr_list();

            if (peek_current_token().type == TOKEN_RPAREN)
            {
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ) after for");
            }

            handle_compound_statement();
        }
        else
        {
            error_handler("Expected: ( after for key.");
        }
    }
}

void handle_expr()
{
    if (PARSER_ERROR) return;
    Token curr_tok = peek_current_token();
    Token next_tok = peek_next_token_no_advance();
    bool has_assign_token = look_ahead(TOKEN_ASSIGN, false, false);

    if (0 == strcmp(curr_tok.value, DEF_VAR) 
        || curr_tok.type == TOKEN_ID 
        && (has_assign_token || next_tok.type == TOKEN_SEMICOLON))
    {
        handle_assign_expr();
    }
    else
    {
        handle_arith_expr();
    }
}

void handle_assign_expr()
{
    if (PARSER_ERROR) return;
    if (0 == strcmp(peek_current_token().value, DEF_VAR))
    {
        peek_next_token();

        if (peek_current_token().type != TOKEN_ID)
        {
            error_handler("Expected: IDENTIFIER after let key");
        }
    }

    if (peek_current_token().type == TOKEN_ID)
    {
        handle_array_atom();
        if (peek_current_token().type == TOKEN_ASSIGN)
        {
            peek_next_token();

            handle_expr();
        }
    }
}

void handle_arith_expr()
{
    if (PARSER_ERROR) return;
    handle_term();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        peek_next_token();
        handle_term();
    }
}

void handle_term()
{
    if (PARSER_ERROR) return;
    handle_factor();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_MUL || peek_current_token().type == TOKEN_DIV)
    {
        peek_next_token();
        handle_factor();
    }
}

void handle_factor()
{
    if (PARSER_ERROR) return;
    while (!PARSER_ERROR && peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        peek_next_token();
    }

    if (peek_current_token().type != TOKEN_POW)
    {
        handle_atom();
    }

    if (peek_current_token().type == TOKEN_POW)
    {
        handle_pow();
    }
}

void handle_items_array()
{
    if (PARSER_ERROR) return;
    handle_atom();

    while (!PARSER_ERROR && peek_current_token().type == TOKEN_COMMA)
    {
        peek_next_token();
        handle_atom();
    }
}

void handle_array()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_LSQUARE)
    {
        peek_next_token();

        if (peek_current_token().type == TOKEN_RSQUARE)
        {
            peek_next_token();
            return;
        }

        handle_items_array();

        if (peek_current_token().type == TOKEN_RSQUARE)
        {
            peek_next_token();
        }
        else
        {
            error_handler("Expected: ]");
        }
    }
}

void handle_array_atom()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_ID)
    {
        peek_next_token();

        if (peek_current_token().type == TOKEN_LSQUARE)
        {
            peek_next_token();

            handle_atom();

            if (peek_current_token().type == TOKEN_RSQUARE)
            {
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ]");
            }
        }
    }
}

void handle_atom()
{
    if (PARSER_ERROR) return;
    Token curr_tok = peek_current_token();
    if (curr_tok.type == TOKEN_LPAREN)
    {
        peek_next_token();

        handle_expr();

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            peek_next_token();
        }
        else
        {
            error_handler("Expected: )");
        }
    }
    else if (curr_tok.type == TOKEN_INTEGER 
        || curr_tok.type == TOKEN_FLOAT 
        || curr_tok.type == TOKEN_STRING
    )
    {
        peek_next_token();
    }
    else if (curr_tok.type == TOKEN_ID)
    {
        handle_array_atom();
    }
    else if (curr_tok.type == TOKEN_LSQUARE)
    {
        handle_array();
    }
    else
    {
        error_handler("Expected: INTEGER | FLOAT | ARRAY | IDENTIFIER | STRING | ( EXPRESSION )");
    }
}

void handle_pow()
{
    if (PARSER_ERROR) return;
    if (peek_current_token().type == TOKEN_POW)
    {
        peek_next_token();

        handle_atom();
    }
    else
    {
        error_handler("Expected: ^ pow");
    }
}
