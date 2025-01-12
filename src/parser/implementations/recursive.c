#include "recursive.h"

ASTNode *create_node_current_token()
{
    Token t = peek_current_token();

    return create_node(t.value, &t);
}

ASTNode *handle_program()
{

    ASTNode *root = create_node("<program>", NULL);

    while (0 != strcmp(peek_current_token().value, E0F))
    {
        ASTNode *handle_statement_node = handle_statement();
        if (handle_statement_node == NULL)
        {
            return handle_statement_node;
        }
        add_child(root, handle_statement_node);
    }

    return root;
}

ASTNode *handle_else()
{
    ASTNode *node = NULL;

    if (0 == strcmp(peek_current_token().value, ELSE))
    {
        node = create_node_current_token();
        peek_next_token();

        add_child(node, handle_compound_statement());
    }

    return node;
}

ASTNode *handle_if()
{

    ASTNode *node = NULL;

    if (0 == strcmp(peek_current_token().value, IF))
    {
        node = create_node_current_token();

        peek_next_token();

        ASTNode *statement_structure = handle_statement_structure();

        if (statement_structure == NULL)
            return NULL;

        add_child(node, statement_structure);

        while (0 == strcmp(peek_current_token().value, ELSEIF))
        {
            add_child(node, handle_elseif());
        }

        if (0 == strcmp(peek_current_token().value, ELSE))
        {
            add_child(node, handle_else());
        }

        if (0 == strcmp(peek_current_token().value, ELSEIF))
        {
            error_handler("Expected: IF before ELSEIF");

            return NULL;
        }
    }

    return node;
}

ASTNode *handle_statement()
{

    ASTNode *node = NULL;

    Token curr_tok = peek_current_token();
    if (0 == strcmp(curr_tok.value, IF))
    {
        return handle_if();
    }
    else if (0 == strcmp(curr_tok.value, WHILE))
    {
        return handle_while();
    }
    else if (0 == strcmp(curr_tok.value, FOR))
    {
        return handle_for();
    }
    else if (curr_tok.type == TOKEN_AND || curr_tok.type == TOKEN_ASSIGN || curr_tok.type == TOKEN_COMMA || curr_tok.type == TOKEN_DIV || curr_tok.type == TOKEN_EQ || curr_tok.type == TOKEN_GE || curr_tok.type == TOKEN_GT || curr_tok.type == TOKEN_LBRACE || curr_tok.type == TOKEN_LE || curr_tok.type == TOKEN_LSQUARE || curr_tok.type == TOKEN_LT || curr_tok.type == TOKEN_MUL || curr_tok.type == TOKEN_NE || curr_tok.type == TOKEN_NOT || curr_tok.type == TOKEN_OR || curr_tok.type == TOKEN_RSQUARE || curr_tok.type == TOKEN_RPAREN || curr_tok.type == TOKEN_RBRACE || curr_tok.type == TOKEN_POW)
    {
        error_handler("Expected: Expr | If | While | For");
        return NULL;
    }
    else
    {

        node = handle_expr();

        Token current_token = peek_current_token();
        add_child(node, create_node(peek_current_token().value, &current_token));

        if (peek_current_token().type == TOKEN_SEMICOLON)
        {
            peek_next_token();
        }
        else
        {
            error_handler("Expected: ;");
            return NULL;
        }
    }

    return node;
}

ASTNode *handle_elseif()
{
    ASTNode *node = create_node_current_token();

    peek_next_token();

    add_child(node, handle_statement_structure());

    return node;
}

ASTNode *handle_while()
{

    ASTNode *node = NULL;
    Token current_token = peek_current_token();

    if (0 == strcmp(current_token.value, WHILE))
    {
        node = create_node("<while_statement>", &current_token);
        peek_next_token();

        ASTNode *statement_structure = handle_statement_structure();

        if (statement_structure == NULL)
            return NULL;

        add_child(node, statement_structure);
    }

    return node;
}

ASTNode *handle_compound_statement()
{

    ASTNode *node = create_node("<compound_statement>", NULL);
    if (peek_current_token().type == TOKEN_LBRACE)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        while (peek_current_token().type != TOKEN_RBRACE && peek_current_token().type != TOKEN_EOF)
        {
            add_child(node, handle_statement());
        }
        if (peek_current_token().type != TOKEN_RBRACE)
        {
            error_handler("Expected: }");
            return NULL;
        }

        add_child(node, create_node_current_token());
        peek_next_token();
    }
    else
    {
        error_handler("Expected: {");
        return NULL;
    }

    return node;
}

ASTNode *handle_expr_bool_op()
{

    ASTNode *node = create_node_current_token();

    if (peek_current_token().type == TOKEN_EQ || peek_current_token().type == TOKEN_LE || peek_current_token().type == TOKEN_GE || peek_current_token().type == TOKEN_NE)
    {
        peek_next_token();
        peek_next_token();
    }
    else if (peek_current_token().type == TOKEN_GT || peek_current_token().type == TOKEN_LT || peek_current_token().type == TOKEN_NOT)
    {
        peek_next_token();
    }

    return node;
}
ASTNode *handle_expr_bool_rel()
{
    ASTNode *node = handle_arith_expr();

    if (peek_current_token().type == TOKEN_EQ || peek_current_token().type == TOKEN_LE || peek_current_token().type == TOKEN_GE || peek_current_token().type == TOKEN_NE || peek_current_token().type == TOKEN_GT || peek_current_token().type == TOKEN_LT)
    {
        add_child(node, handle_expr_bool_rel_tail());
    }

    return node;
}

ASTNode *handle_expr_bool_rel_tail()
{
    ASTNode *node = create_node_current_token();

    peek_next_token();

    add_child(node, handle_arith_expr());

    return node;
}

ASTNode *handle_expr_bool_not()
{
    ASTNode *node = create_node("<expr_bool_not>", NULL);

    if (peek_current_token().type == TOKEN_NOT)
    {
        add_child(node, create_node_current_token());
        peek_next_token();
    }

    if (peek_current_token().type == TOKEN_LPAREN)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_expr_bool());

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            add_child(node, create_node_current_token());
            peek_next_token();
        }
        else
        {
            error_handler("Expected: )");
            return NULL;
        }
    }
    else
    {
        add_child(node, handle_expr_bool_rel());
    }

    return node;
}

ASTNode *handle_expr_bool_and()
{
    ASTNode *node = handle_expr_bool_not();

    while (peek_current_token().type == TOKEN_AND)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_expr_bool_not());
    }

    return node;
}

ASTNode *handle_expr_bool_or()
{

    ASTNode *node = handle_expr_bool_and();

    while (peek_current_token().type == TOKEN_OR)
    {
        add_child(node, create_node_current_token());
        peek_next_token();

        add_child(node, handle_expr_bool_and());
    }

    return node;
}

ASTNode *handle_expr_bool()
{

    ASTNode *node = handle_expr_bool_or();

    return node;
}

ASTNode *handle_statement_structure()
{
    ASTNode *node = create_node("<statement_structure>", NULL);
    Token current_token = peek_current_token();

    if (current_token.type == TOKEN_LPAREN)
    {
        add_child(node, create_node(current_token.value, &current_token));
        peek_next_token();

        if (peek_current_token().type == TOKEN_RPAREN)
            error_handler("Expected: bool expression");

        add_child(node, handle_expr_bool());

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            add_child(node, create_node_current_token());
            peek_next_token();

            ASTNode *compound_statement = handle_compound_statement();

            if (compound_statement == NULL)
                return NULL;

            add_child(node, compound_statement);
        }
        else
        {
            error_handler("Expected: )");
            return NULL;
        }
    }
    else
    {
        error_handler("Expected: ()");
        return NULL;
    }

    return node;
}

ASTNode *handle_assign_expr_tail()
{
    ASTNode *node = handle_reassign_expr();

    while (peek_current_token().type == TOKEN_COMMA)
    {
        add_child(node, create_node_current_token());
        peek_next_token();

        add_child(node, handle_reassign_expr());
    }

    return node;
}

ASTNode *handle_assign_expr_list()
{
    ASTNode *node = handle_reassign_expr();

    if (peek_current_token().type == TOKEN_COMMA)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_assign_expr_tail());
    }

    return node;
}

ASTNode *handle_for()
{

    ASTNode *node = create_node("<for>", NULL);
    if (0 == strcmp(peek_current_token().value, FOR))
    {
        add_child(node, create_node_current_token());
        peek_next_token();

        if (peek_current_token().type == TOKEN_LPAREN)
        {
            add_child(node, create_node_current_token());
            peek_next_token();

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                add_child(node, handle_assign_expr_list());

            if (peek_current_token().type == TOKEN_SEMICOLON)
            {
                add_child(node, create_node_current_token());
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ; after assign expr list in for");
                return NULL;
            }

            if (peek_current_token().type != TOKEN_SEMICOLON)
                add_child(node, handle_expr_bool());

            if (peek_current_token().type == TOKEN_SEMICOLON)
            {
                add_child(node, create_node_current_token());
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ; after boolean expression in for");
                return NULL;
            }

            if (0 == strcmp(peek_current_token().value, DEF_VAR) || peek_current_token().type == TOKEN_ID)
                add_child(node, handle_assign_expr_list());

            if (peek_current_token().type == TOKEN_RPAREN)
            {
                add_child(node, create_node_current_token());
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ) after for");
                return NULL;
            }

            add_child(node, handle_compound_statement());
        }
        else
        {
            error_handler("Expected: ( after for key.");
            return NULL;
        }
    }

    return node;
}

ASTNode *handle_expr()
{

    ASTNode *node = NULL;
    Token curr_tok = peek_current_token();

    if (0 == strcmp(curr_tok.value, DEF_VAR))
    {
        node = handle_decl_expr();
    }
    else if (curr_tok.type == TOKEN_ID)
    {
        peek_next_token();
        node = handle_reassign_expr_tail();
    }

    return node;
}

ASTNode *handle_reassign_expr_tail()
{
    ASTNode *node = NULL;

    if (peek_current_token().type == TOKEN_ASSIGN)
    {
        node = create_node_current_token();

        peek_next_token();

        add_child(node, handle_arith_expr());
    }
    else
    {
        node = handle_arith_expr_tail();
    }

    return node;
}

ASTNode *handle_decl_expr()
{

    ASTNode *node = create_node("<assign_expr>", NULL);

    if (0 == strcmp(peek_current_token().value, DEF_VAR))
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        if (peek_current_token().type != TOKEN_ID)
        {
            error_handler("Expected: IDENTIFIER after let key");
        }

        add_child(node, handle_array_atom());

        if (peek_current_token().type == TOKEN_ASSIGN)
        {
            add_child(node, create_node_current_token());
            peek_next_token();

            add_child(node, handle_arith_expr());
        }
    }

    return node;
}

ASTNode *handle_reassign_expr()
{
    ASTNode *node = NULL;
    if (peek_current_token().type == TOKEN_ID)
    {
        node = handle_array_atom();

        if (peek_current_token().type != TOKEN_ASSIGN)
        {
            error_handler("Expected: ASSIGN after identifier");
        }

        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_arith_expr());
    }

    return node;
}

ASTNode *handle_arith_expr()
{

    ASTNode *node = handle_term();

    if (peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        add_child(node, handle_arith_expr_tail());
    }

    return node;
}

ASTNode *handle_arith_expr_tail()
{
    ASTNode *node = NULL;

    if (peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        node = create_node_current_token();
        peek_next_token();

        add_child(node, handle_term());
    }

    if (peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        node = handle_arith_expr_tail();
    }

    return node;
}

ASTNode *handle_term()
{

    ASTNode *node = handle_factor();

    while (peek_current_token().type == TOKEN_MUL || peek_current_token().type == TOKEN_DIV)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_factor());
    }

    return node;
}

ASTNode *handle_factor()
{

    ASTNode *node = create_node("<factor>", NULL);

    while (peek_current_token().type == TOKEN_PLUS || peek_current_token().type == TOKEN_MINUS)
    {
        add_child(node, create_node_current_token());
        peek_next_token();
    }

    if (peek_current_token().type != TOKEN_POW)
    {
        add_child(node, handle_atom());
    }

    if (peek_current_token().type == TOKEN_POW)
    {
        add_child(node, handle_pow());
    }

    return node;
}

ASTNode *handle_items_array()
{

    ASTNode *node = create_node("<items_array>", NULL);

    add_child(node, handle_atom());

    while (peek_current_token().type == TOKEN_COMMA)
    {
        add_child(node, create_node_current_token());
        peek_next_token();
        add_child(node, handle_atom());
    }

    return node;
}

ASTNode *handle_array()
{

    ASTNode *node = create_node("<array>", NULL);

    if (peek_current_token().type == TOKEN_LSQUARE)
    {
        add_child(node, create_node_current_token());
        peek_next_token();

        if (peek_current_token().type == TOKEN_RSQUARE)
        {
            add_child(node, create_node_current_token());
            peek_next_token();
            return node;
        }

        add_child(node, handle_items_array());

        if (peek_current_token().type == TOKEN_RSQUARE)
        {
            add_child(node, create_node_current_token());
            peek_next_token();
        }
        else
        {
            error_handler("Expected: ]");
            return NULL;
        }
    }

    return node;
}

ASTNode *handle_array_atom()
{
    ASTNode *node = NULL;
    if (peek_current_token().type == TOKEN_ID)
    {
        ASTNode *id_node = create_node_current_token();
        node = id_node;

        peek_next_token();

        if (peek_current_token().type == TOKEN_LSQUARE)
        {
            node = create_node("<array_atom>", NULL);

            add_child(node, id_node);

            peek_next_token();

            add_child(node, handle_atom());

            if (peek_current_token().type == TOKEN_RSQUARE)
            {
                add_child(node, create_node_current_token());
                peek_next_token();
            }
            else
            {
                error_handler("Expected: ]");
                return NULL;
            }
        }
    }

    return node;
}

ASTNode *handle_atom()
{
    Token curr_tok = peek_current_token();
    ASTNode *node = create_node("<atom>", NULL);

    if (curr_tok.type == TOKEN_LPAREN)
    {
        add_child(node, create_node_current_token());

        peek_next_token();

        add_child(node, handle_expr());

        if (peek_current_token().type == TOKEN_RPAREN)
        {
            add_child(node, create_node_current_token());
            peek_next_token();
        }
        else
        {
            error_handler("Expected: )");
            return NULL;
        }
    }
    else if (curr_tok.type == TOKEN_INTEGER || curr_tok.type == TOKEN_FLOAT || curr_tok.type == TOKEN_STRING)
    {
        add_child(node, create_node_current_token());
        peek_next_token();
    }
    else if (curr_tok.type == TOKEN_ID)
    {
        add_child(node, handle_array_atom());
    }
    else if (curr_tok.type == TOKEN_LSQUARE)
    {
        add_child(node, handle_array());
    }
    else
    {
        error_handler("Expected: INTEGER | FLOAT | ARRAY | IDENTIFIER | STRING | ( EXPRESSION )");
        return NULL;
    }

    return node;
}

ASTNode *handle_pow()
{

    ASTNode *node = NULL;
    if (peek_current_token().type == TOKEN_POW)
    {
        node = create_node_current_token();

        peek_next_token();

        add_child(node, handle_atom());
    }
    else
    {
        error_handler("Expected: ^ pow");
        return NULL;
    }

    return node;
}
