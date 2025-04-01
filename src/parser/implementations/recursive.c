#include "recursive.h"
extern int success;

ASTNode *create_node_current_token()
{
    Token t = peek_current_token();

    return create_node(t.value, &t);
}

ASTNode *handle_program()
{
    ASTNode *root = create_node("program", NULL);

    add_child(root, handle_statement_list());

    return root;
}

ASTNode *handle_statement_list()
{
    ASTNode *root = handle_statement();

    add_child(root, handle_statement_list_tail());

    return root;
}

ASTNode *handle_statement()
{

    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermIf)
    {
        return handle_if_statement();
    }
    else if (curr_tok.subtype == TermWhile)
    {
        return handle_while_statement();
    }
    else if (curr_tok.subtype == TermFor)
    {
        return handle_for_statement();
    }
    else if (curr_tok.type == TOKEN_AND || curr_tok.type == TOKEN_ASSIGN || curr_tok.type == TOKEN_COMMA || curr_tok.type == TOKEN_DIV || curr_tok.type == TOKEN_EQ || curr_tok.type == TOKEN_GE || curr_tok.type == TOKEN_GT || curr_tok.type == TOKEN_LBRACE || curr_tok.type == TOKEN_LE || curr_tok.type == TOKEN_LSQUARE || curr_tok.type == TOKEN_LT || curr_tok.type == TOKEN_MUL || curr_tok.type == TOKEN_NE || curr_tok.type == TOKEN_NOT || curr_tok.type == TOKEN_OR || curr_tok.type == TOKEN_RSQUARE || curr_tok.type == TOKEN_RPAREN || curr_tok.type == TOKEN_RBRACE || curr_tok.type == TOKEN_POW)
    {
        error_handler("Expected: Expr | If | While | For");
        return NULL;
    }
    else
    {
        ASTNode *root = handle_expr();

        add_child(root, create_node_current_token());

        if (peek_current_token().type == TOKEN_SEMICOLON)
        {
            peek_next_token();
            return root;
        }
        else
        {
            error_handler("Expected: ;");
            return NULL;
        }
    }
}

ASTNode *handle_statement_list_tail()
{
    if (peek_current_token().subtype == TermDefVar || peek_current_token().subtype == TermIdentifier || peek_current_token().subtype == TermWhile || peek_current_token().subtype == TermFor || peek_current_token().subtype == TermIf || peek_current_token().subtype == TermDefVar)
    {
        ASTNode *root = handle_statement();
        add_child(root, handle_statement_list_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_compound_statement()
{
    if (peek_current_token().subtype == TermLbrace)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_statement_list());

        if (peek_current_token().subtype != TermRbrace)
        {
            error_handler("Expected: }");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());
            peek_next_token();

            return root;
        }
    }
    else
    {
        error_handler("Expected: {");
        return NULL;
    }
}

ASTNode *handle_expr()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *root = handle_identifier();

        add_child(root, handle_reassign_expr_tail());

        return root;
    }
    else if (peek_current_token().subtype == TermDefVar)
    {
        return handle_decl_expr();
    }
    else
    {
        error_handler("expected: let | id");
        return NULL;
    }
}

ASTNode *handle_reassign_expr()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *root = handle_identifier();

        if (peek_current_token().subtype != TermAssign)
        {
            error_handler("expected: = assign");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());
            peek_next_token();

            add_child(root, handle_arith_expr());

            return root;
        }
    }
    else
    {
        error_handler("expected: id");
        return NULL;
    }
}

ASTNode *handle_decl_expr()
{
    if (peek_current_token().subtype == TermDefVar)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        if (peek_current_token().subtype != TermIdentifier)
        {
            error_handler("expected: id");
            return NULL;
        }
        else
        {
            add_child(root, handle_identifier());
            add_child(root, handle_decl_expr_assign());

            return root;
        }
    }
    else
    {
        error_handler("expected let");
        return NULL;
    }
}

ASTNode *handle_decl_expr_assign()
{
    if (peek_current_token().subtype == TermAssign)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();
        add_child(root, handle_arith_expr());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_reassign_expr_tail()
{
    if (peek_current_token().subtype == TermAssign)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        add_child(root, handle_arith_expr());

        return root;
    }
    else
    {
        return handle_arith_expr_tail();
    }
}

ASTNode *handle_arith_expr()
{
    ASTNode *root = handle_term();

    add_child(root, handle_arith_expr_tail());

    return root;
}

ASTNode *handle_arith_expr_tail()
{
    return handle_arith_expr_tail_rest();
}

ASTNode *handle_arith_expr_tail_rest()
{
    if (peek_current_token().subtype == TermPlus)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        add_child(root, handle_term());

        add_child(root, handle_arith_expr_tail_rest());

        return root;
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        add_child(root, handle_term());

        add_child(root, handle_arith_expr_tail_rest());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_term()
{
    ASTNode *root = handle_factor();

    add_child(root, handle_term_list());

    return root;
}

ASTNode *handle_term_list()
{
    return handle_term_list_tail();
}

ASTNode *handle_term_list_tail()
{
    if (peek_current_token().subtype == TermMul)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        add_child(root, handle_factor());

        add_child(root, handle_term_list_tail());

        return root;
    }
    else if (peek_current_token().subtype == TermDiv)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        add_child(root, handle_factor());

        add_child(root, handle_term_list_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_factor()
{
    return handle_factor_rest();
}

ASTNode *handle_factor_rest()
{
    if (peek_current_token().subtype == TermPlus)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_pow());

        return root;
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_pow());

        return root;
    }
    else
    {
        return handle_pow();
    }
}

ASTNode *handle_pow()
{
    ASTNode *root = handle_atom();
    add_child(root, handle_pow_rest());

    return root;
}

ASTNode *handle_pow_rest()
{
    if (peek_current_token().subtype == TermPow)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_pow());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_atom()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        return handle_identifier();
    }
    else if (peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        return root;
    }
    else if (peek_current_token().subtype == TermLparen)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_arith_expr());

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler("expected: )");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());
            peek_next_token();
            return root;
        }
    }
    else if (peek_current_token().subtype == TermLbracket)
    {
        return handle_array();
    }
    else
    {
        error_handler("expected Atom");
        return NULL;
    }
}

ASTNode *handle_array()
{
    if (peek_current_token().subtype == TermLbracket)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();
        add_child(root, handle_items_array());

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("expected: []");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());
            peek_next_token();

            return root;
        }
    }
    else
    {
        error_handler("expected: ]");
        return NULL;
    }
}

ASTNode *handle_items_array()
{
    if (peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *root = handle_atom();

        add_child(root, handle_items_array_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_items_array_tail()
{
    if (peek_current_token().subtype == TermComma)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_atom());

        add_child(root, handle_items_array_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_expr_bool()
{
    return handle_expr_bool_or();
}

ASTNode *handle_expr_bool_or()
{
    ASTNode *root = handle_expr_bool_and();

    add_child(root, handle_expr_bool_or_rest());

    return root;
}

ASTNode *handle_expr_bool_or_rest()
{
    if (peek_current_token().subtype == TermOr)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_expr_bool_and());

        add_child(root, handle_expr_bool_and_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_expr_bool_and()
{
    ASTNode *root = handle_expr_bool_not();

    add_child(root, handle_expr_bool_and_tail());

    return root;
}

ASTNode *handle_expr_bool_and_tail()
{
    if (peek_current_token().subtype == TermAnd)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_expr_bool_not());

        add_child(root, handle_expr_bool_and_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_expr_bool_not()
{
    if (peek_current_token().subtype == TermNot)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_expr_bool_not());
        return root;
    }
    else
    {
        return handle_expr_bool_rel();
    }
}

ASTNode *handle_expr_bool_rel()
{
    ASTNode *root = handle_arith_expr();

    add_child(root, handle_expr_bool_rel_tail());

    return root;
}

ASTNode *handle_expr_bool_rel_tail()
{
    if (peek_current_token().subtype == TermLe || peek_current_token().subtype == TermGe || peek_current_token().subtype == TermGt || peek_current_token().subtype == TermLt || peek_current_token().subtype == TermNe || peek_current_token().subtype == TermEq)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_arith_expr());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_expr_bool_rel_factor()
{
    return handle_arith_expr();
}

ASTNode *handle_while_statement()
{
    if (peek_current_token().subtype == TermWhile)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_statement_structure());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_if_statement()
{
    if (peek_current_token().subtype == TermIf)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_statement_structure());
        add_child(root, handle_else_if_statement_list());

        add_child(root, handle_else_statement());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_else_if_statement_list()
{
    return handle_elseif_statement_tail();
}

ASTNode *handle_else_if_statement()
{
    if (peek_current_token().subtype == TermElseIf)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_statement_structure());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_elseif_statement_tail()
{
    if (peek_current_token().subtype == TermElseIf)
    {
        ASTNode *root = handle_else_if_statement();

        add_child(root, handle_elseif_statement_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_else_statement()
{
    if (peek_current_token().subtype == TermElse)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_compound_statement());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_statement_structure()
{
    if (peek_current_token().subtype == TermLparen)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_expr_bool());

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler("expected:)");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());

            peek_next_token();

            add_child(root, handle_compound_statement());

            return root;
        }
    }
    else
    {
        error_handler("expected :(");
        return NULL;
    }
}

ASTNode *handle_for_statement()
{
    if (peek_current_token().subtype == TermFor)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        if (peek_current_token().subtype != TermLparen)
        {
            error_handler("expected: ()");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());

            peek_next_token();

            add_child(root, handle_assign_expr_list());

            if (peek_current_token().subtype != TermSemi)
            {
                error_handler("expected: ; 2");
                return NULL;
            }
            else
            {
                add_child(root, create_node_current_token());

                peek_next_token();
                add_child(root, handle_for_expr_bool());
                if (peek_current_token().subtype != TermSemi)
                {
                    error_handler("expected: ; 3");
                    return NULL;
                }
                else
                {
                    add_child(root, create_node_current_token());

                    peek_next_token();

                    add_child(root, handle_assign_expr_list());

                    if (peek_current_token().subtype != TermRparen)
                    {
                        error_handler("expected: )");
                        return NULL;
                    }
                    else
                    {
                        add_child(root, create_node_current_token());

                        peek_next_token();
                        add_child(root, handle_compound_statement());

                        return root;
                    }
                }
            }
        }
    }

    return create_node("empty", NULL);
}

ASTNode *handle_for_expr_bool()
{
    if (peek_current_token().subtype == TermNot || peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        return handle_expr_bool();
    }

    return create_node("empty", NULL);
}

ASTNode *handle_assign_expr_list()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *root = handle_reassign_expr();

        add_child(root, handle_assign_expr_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_assign_expr_tail()
{
    if (peek_current_token().subtype == TermComma)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_reassign_expr());

        add_child(root, handle_assign_expr_tail());

        return root;
    }

    return create_node("empty", NULL);
}

ASTNode *handle_identifier()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_identifier_array());

        return root;
    }
    else
    {
        error_handler("expected id");
        return NULL;
    }
}

ASTNode *handle_identifier_array()
{
    if (peek_current_token().subtype == TermLbracket)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_arith_expr());

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("expected arrauyyp");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());

            peek_next_token();

            return root;
        }
    }

    return create_node("empty", NULL);
}
