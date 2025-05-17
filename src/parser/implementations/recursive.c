#include "recursive.h"
extern int success;

ASTNode *create_node_current_token()
{
    Token t = peek_current_token();

    return create_node(t.value, t);
}

ASTNode *handle_program()
{
    ASTNode *root = create_node("Program", get_undef_token());

    handle_statement_list(root);

    return root;
}

ASTNode *handle_statement_list(ASTNode *root)
{
    add_child(root, handle_statement());
    return handle_statement_list_tail(root);
}

ASTNode *handle_statement()
{
    ASTNode *root = NULL;
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
        root = handle_expr();

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

    return root;
}

ASTNode *handle_statement_list_tail(ASTNode *root)
{
    if (peek_current_token().subtype == TermIdentifier || peek_current_token().subtype == TermWhile || peek_current_token().subtype == TermFor || peek_current_token().subtype == TermIf || peek_current_token().subtype == TermDefVar)
    {
        add_child(root, handle_statement());

        return handle_statement_list_tail(root);
    }

    return root;
}

ASTNode *handle_compound_statement()
{
    ASTNode *block_node = create_node("block", get_undef_token());

    if (peek_current_token().subtype == TermLbrace)
    {
        peek_next_token();

        handle_statement_list(block_node);

        if (peek_current_token().subtype != TermRbrace)
        {
            error_handler("Expected: }");
            return NULL;
        }

        peek_next_token();
    }
    else
    {
        error_handler("Expected: {");
        return NULL;
    }

    return block_node;
}

ASTNode *handle_expr()
{
    ASTNode *node = NULL;
    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermIdentifier)
    {
        node = handle_reassign_expr_tail(handle_identifier());
    }
    else if (peek_current_token().subtype == TermDefVar)
    {
        node = handle_decl_expr();
    }
    else
    {
        error_handler("expected: let | id");
        return NULL;
    }

    return node;
}

ASTNode *handle_reassign_expr()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        ASTNode *r = create_node("=", get_undef_token());
        add_child(r, handle_identifier());

        if (peek_current_token().subtype != TermAssign)
        {
            error_handler("expected: = assign");
            return NULL;
        }
        else
        {
            peek_next_token();

            add_child(r, handle_arith_expr());

            return r;
        }
    }
    else
    {
        error_handler("Expected: Identifier");
        return NULL;
    }
}

ASTNode *handle_decl_expr()
{
    ASTNode *node = NULL;

    if (peek_current_token().subtype == TermDefVar)
    {
        node = create_node_current_token();
        peek_next_token();

        if (peek_current_token().subtype != TermIdentifier)
        {
            error_handler("Expected: Identifier");
            return NULL;
        }
        else
        {
            add_child(node, handle_identifier());
            add_child(node, handle_decl_expr_assign());
        }
    }
    else
    {
        error_handler("expected let");
        return NULL;
    }

    return node;
}

ASTNode *handle_decl_expr_assign()
{
    ASTNode *node = NULL;
    if (peek_current_token().subtype == TermAssign)
    {
        peek_next_token();
        node = handle_arith_expr();
    }

    return node;
}

ASTNode *handle_reassign_expr_tail(ASTNode *lhs)
{
    if (peek_current_token().subtype == TermAssign)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, lhs);
        add_child(root, handle_arith_expr());

        return root;
    }

    return handle_arith_expr_tail();
}

ASTNode *handle_arith_expr()
{
    return handle_arith_expr_tail(handle_term());
}

ASTNode *handle_arith_expr_tail(ASTNode *term)
{
    return handle_arith_expr_tail_rest(term);
}

ASTNode *handle_arith_expr_tail_rest(ASTNode *lhs)
{
    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermPlus || curr_tok.subtype == TermMinus)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        ASTNode *rhs = handle_term();

        add_child(root, lhs);
        add_child(root, rhs);

        return handle_arith_expr_tail_rest(root);
    }

    return lhs;
}

ASTNode *handle_term()
{
    return handle_term_list(handle_factor());
    // ASTNode *root = handle_factor();

    // add_child(root, handle_term_list());

    // return root;
}

ASTNode *handle_term_list(ASTNode *lhs)
{
    return handle_term_list_tail(lhs);
}

ASTNode *handle_term_list_tail(ASTNode *lhs)
{
    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermMul || curr_tok.subtype == TermDiv)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();

        ASTNode *rhs = handle_factor();
        add_child(root, lhs);
        add_child(root, rhs);

        return handle_term_list_tail(root);
    }

    return lhs;
}

ASTNode *handle_factor()
{
    return handle_factor_rest();
}

ASTNode *handle_factor_rest()
{
    Token curr_tok = peek_current_token();

    if (curr_tok.subtype == TermPlus || curr_tok.subtype == TermMinus)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, handle_pow());

        return root;
    }

    return handle_pow();
}

ASTNode *handle_pow()
{
    return handle_pow_rest(handle_atom());
}

ASTNode *handle_pow_rest(ASTNode *lhs)
{
    if (peek_current_token().subtype == TermPow)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        add_child(root, lhs);
        add_child(root, handle_pow());

        return root;
    }

    return lhs;
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
        ASTNode *root = create_node("[]", peek_current_token());

        peek_next_token();
        handle_items_array(root);

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("expected: []");
            return NULL;
        }
        else
        {
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

ASTNode *handle_items_array(ASTNode *root)
{
    if (peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        add_child(root, handle_atom());

        return handle_items_array_tail(root);
    }

    return root;
}

ASTNode *handle_items_array_tail(ASTNode *root)
{
    if (peek_current_token().subtype == TermComma)
    {
        peek_next_token();

        add_child(root, handle_atom());

        return handle_items_array_tail(root);
    }

    return root;
}

ASTNode *handle_expr_bool()
{
    return handle_expr_bool_or();
}

ASTNode *handle_expr_bool_or()
{
    return handle_expr_bool_or_rest(handle_expr_bool_and());
}

ASTNode *handle_expr_bool_or_rest(ASTNode *lhs)
{
    if (peek_current_token().subtype == TermOr)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();
        ASTNode *rhs = handle_expr_bool_and();
        add_child(root, lhs);
        add_child(root, handle_expr_bool_or_rest(rhs));
        return root;
    }
    return lhs;
}

ASTNode *handle_expr_bool_and()
{
    ASTNode *lhs = handle_expr_bool_not();
    return handle_expr_bool_and_tail(lhs);
}

ASTNode *handle_expr_bool_and_tail(ASTNode *lhs)
{
    if (peek_current_token().subtype == TermAnd)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();
        ASTNode *rhs = handle_expr_bool_not();
        add_child(root, lhs);
        add_child(root, handle_expr_bool_and_tail(rhs));
        return root;
    }
    return lhs;
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
    return handle_expr_bool_rel();
}

ASTNode *handle_expr_bool_rel()
{
    ASTNode *lhs = handle_arith_expr();
    return handle_expr_bool_rel_tail(lhs);
}

ASTNode *handle_expr_bool_rel_tail(ASTNode *lhs)
{
    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermLe || curr_tok.subtype == TermGe || curr_tok.subtype == TermGt || curr_tok.subtype == TermLt || curr_tok.subtype == TermNe || curr_tok.subtype == TermEq)
    {
        ASTNode *root = create_node_current_token();
        peek_next_token();
        ASTNode *rhs = handle_arith_expr();
        add_child(root, lhs);
        add_child(root, rhs);
        return root;
    }
    return lhs;
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

    return NULL;
}

ASTNode *handle_if_statement()
{
    if (peek_current_token().subtype == TermIf)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        handle_statement_structure(root);
        handle_else_if_statement_list(root);

        handle_else_statement(root);

        return root;
    }

    return NULL;
}

ASTNode *handle_else_if_statement_list(ASTNode *root)
{
    return handle_elseif_statement_tail(root);
}

ASTNode *handle_else_if_statement()
{
    if (peek_current_token().subtype == TermElseIf)
    {
        ASTNode *root = create_node_current_token();

        peek_next_token();

        handle_statement_structure(root);

        return root;
    }

    return NULL;
}

ASTNode *handle_elseif_statement_tail(ASTNode *root)
{
    if (peek_current_token().subtype == TermElseIf)
    {
        add_child(root, handle_else_if_statement());

        return handle_elseif_statement_tail(root);
    }

    return root;
}

ASTNode *handle_else_statement(ASTNode *root)
{
    if (peek_current_token().subtype == TermElse)
    {
        ASTNode *root_else = create_node_current_token();

        peek_next_token();

        add_child(root_else, handle_compound_statement());
        add_child(root, root_else);
        return root;
    }

    return NULL;
}

ASTNode *handle_statement_structure(ASTNode *root)
{
    if (peek_current_token().subtype == TermLparen)
    {
        peek_next_token();

        add_child(root, handle_expr_bool());

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler("expected:)");
            return NULL;
        }
        else
        {
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
            peek_next_token();
            ASTNode *assigns_node = create_node("assigns", get_undef_token());

            handle_assign_expr_list(assigns_node);

            add_child(root, assigns_node);
            if (peek_current_token().subtype != TermSemi)
            {
                error_handler("expected: ; 2");
                return NULL;
            }
            else
            {
                peek_next_token();
                ASTNode *exprs_node = create_node("expr", get_undef_token());
                add_child(exprs_node, handle_for_expr_bool());
                add_child(root, exprs_node);
                if (peek_current_token().subtype != TermSemi)
                {
                    error_handler("expected: ; 3");
                    return NULL;
                }
                else
                {
                    peek_next_token();

                    ASTNode *assigns_node = create_node("assigns", get_undef_token());

                    handle_assign_expr_list(assigns_node);
                    add_child(root, assigns_node);

                    if (peek_current_token().subtype != TermRparen)
                    {
                        error_handler("expected: )");
                        return NULL;
                    }
                    else
                    {
                        peek_next_token();
                        add_child(root, handle_compound_statement());

                        return root;
                    }
                }
            }
        }
    }

    return NULL;
}

ASTNode *handle_for_expr_bool()
{
    if (peek_current_token().subtype == TermNot || peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        return handle_expr_bool();
    }

    return NULL;
}

ASTNode *handle_assign_expr_list(ASTNode *assigns_node)
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child(assigns_node, handle_reassign_expr());

        handle_assign_expr_tail(assigns_node);

        return assigns_node;
    }

    return NULL;
}

ASTNode *handle_assign_expr_tail(ASTNode *assigns_node)
{
    if (peek_current_token().subtype == TermComma)
    {
        peek_next_token();

        add_child(assigns_node, handle_reassign_expr());

        handle_assign_expr_tail(assigns_node);
    }

    return assigns_node;
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
        error_handler("Expected: Identifierentifier");
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
            error_handler("Expected: ]");
            return NULL;
        }
        else
        {
            add_child(root, create_node_current_token());

            peek_next_token();

            return root;
        }
    }

    return NULL;
}


/*
 *
 *  Abaixo, o mesmo parser, porem apenas fazendo a checagem de sintaxe, sem gerar ast. 
 * 
*/

ASTNode* only_syntax_check_handle_program()
{
    create_node("program", get_undef_token());

    only_syntax_check_handle_statement_list();

    return NULL;
}

void only_syntax_check_handle_statement_list()
{
    only_syntax_check_handle_statement();

    only_syntax_check_handle_statement_list_tail();

    return;
}

void only_syntax_check_handle_statement()
{

    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermIf)
    {
        return only_syntax_check_handle_if_statement();
    }
    else if (curr_tok.subtype == TermWhile)
    {
        return only_syntax_check_handle_while_statement();
    }
    else if (curr_tok.subtype == TermFor)
    {
        return only_syntax_check_handle_for_statement();
    }
    else if (curr_tok.type == TOKEN_AND || curr_tok.type == TOKEN_ASSIGN || curr_tok.type == TOKEN_COMMA || curr_tok.type == TOKEN_DIV || curr_tok.type == TOKEN_EQ || curr_tok.type == TOKEN_GE || curr_tok.type == TOKEN_GT || curr_tok.type == TOKEN_LBRACE || curr_tok.type == TOKEN_LE || curr_tok.type == TOKEN_LSQUARE || curr_tok.type == TOKEN_LT || curr_tok.type == TOKEN_MUL || curr_tok.type == TOKEN_NE || curr_tok.type == TOKEN_NOT || curr_tok.type == TOKEN_OR || curr_tok.type == TOKEN_RSQUARE || curr_tok.type == TOKEN_RPAREN || curr_tok.type == TOKEN_RBRACE || curr_tok.type == TOKEN_POW)
    {
        error_handler("Expected: Expr | If | While | For");
        return NULL;
    }
    else
    {
        only_syntax_check_handle_expr();

        create_node_current_token();

        if (peek_current_token().type == TOKEN_SEMICOLON)
        {
            peek_next_token();
            return;
        }
        else
        {
            error_handler("Expected: ;");
            return NULL;
        }
    }
}

void only_syntax_check_handle_statement_list_tail()
{
    if (peek_current_token().subtype == TermDefVar || peek_current_token().subtype == TermIdentifier || peek_current_token().subtype == TermWhile || peek_current_token().subtype == TermFor || peek_current_token().subtype == TermIf || peek_current_token().subtype == TermDefVar)
    {
        only_syntax_check_handle_statement();
        only_syntax_check_handle_statement_list_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_compound_statement()
{
    if (peek_current_token().subtype == TermLbrace)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_statement_list();

        if (peek_current_token().subtype != TermRbrace)
        {
            error_handler("Expected: }");
            return NULL;
        }
        else
        {
            create_node_current_token();
            peek_next_token();

            return;
        }
    }
    else
    {
        error_handler("Expected: {");
        return NULL;
    }
}

void only_syntax_check_handle_expr()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        only_syntax_check_handle_identifier();

        only_syntax_check_handle_reassign_expr_tail();

        return;
    }
    else if (peek_current_token().subtype == TermDefVar)
    {
        return only_syntax_check_handle_decl_expr();
    }
    else
    {
        error_handler("expected: let | id");
        return NULL;
    }
}

void only_syntax_check_handle_reassign_expr()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        only_syntax_check_handle_identifier();

        if (peek_current_token().subtype != TermAssign)
        {
            error_handler("expected: = assign");
            return NULL;
        }
        else
        {
            create_node_current_token();
            peek_next_token();

            only_syntax_check_handle_arith_expr();

            return;
        }
    }
    else
    {
        error_handler("Expected: Identifier");
        return NULL;
    }
}

void only_syntax_check_handle_decl_expr()
{
    if (peek_current_token().subtype == TermDefVar)
    {
        create_node_current_token();
        peek_next_token();

        if (peek_current_token().subtype != TermIdentifier)
        {
            error_handler("Expected: Identifier");
            return NULL;
        }
        else
        {
            only_syntax_check_handle_identifier();
            only_syntax_check_handle_decl_expr_assign();

            return;
        }
    }
    else
    {
        error_handler("expected let");
        return NULL;
    }
}

void only_syntax_check_handle_decl_expr_assign()
{
    if (peek_current_token().subtype == TermAssign)
    {
        create_node_current_token();
        peek_next_token();
        only_syntax_check_handle_arith_expr();

        return;
    }

    return;
}

void only_syntax_check_handle_reassign_expr_tail()
{
    if (peek_current_token().subtype == TermAssign)
    {
        create_node_current_token();
        peek_next_token();

        only_syntax_check_handle_arith_expr();

        return;
    }
    else
    {
        return only_syntax_check_handle_arith_expr_tail();
    }
}

void only_syntax_check_handle_arith_expr()
{
    only_syntax_check_handle_term();

    only_syntax_check_handle_arith_expr_tail();

    return;
}

void only_syntax_check_handle_arith_expr_tail()
{
    return only_syntax_check_handle_arith_expr_tail_rest();
}

void only_syntax_check_handle_arith_expr_tail_rest()
{
    if (peek_current_token().subtype == TermPlus)
    {
        create_node_current_token();
        peek_next_token();

        only_syntax_check_handle_term();

        only_syntax_check_handle_arith_expr_tail_rest();

        return;
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        create_node_current_token();
        peek_next_token();

        only_syntax_check_handle_term();

        only_syntax_check_handle_arith_expr_tail_rest();

        return;
    }

    return;
}

void only_syntax_check_handle_term()
{
    only_syntax_check_handle_factor();

    only_syntax_check_handle_term_list();

    return;
}

void only_syntax_check_handle_term_list()
{
    return only_syntax_check_handle_term_list_tail();
}

void only_syntax_check_handle_term_list_tail()
{
    if (peek_current_token().subtype == TermMul)
    {
        create_node_current_token();
        peek_next_token();

        only_syntax_check_handle_factor();

        only_syntax_check_handle_term_list_tail();

        return;
    }
    else if (peek_current_token().subtype == TermDiv)
    {
        create_node_current_token();
        peek_next_token();

        only_syntax_check_handle_factor();

        only_syntax_check_handle_term_list_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_factor()
{
    return only_syntax_check_handle_factor_rest();
}

void only_syntax_check_handle_factor_rest()
{
    if (peek_current_token().subtype == TermPlus)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_pow();

        return;
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_pow();

        return;
    }
    else
    {
        return only_syntax_check_handle_pow();
    }
}

void only_syntax_check_handle_pow()
{
    only_syntax_check_handle_atom();
    only_syntax_check_handle_pow_rest();

    return;
}

void only_syntax_check_handle_pow_rest()
{
    if (peek_current_token().subtype == TermPow)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_pow();

        return;
    }

    return;
}

void only_syntax_check_handle_atom()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        return only_syntax_check_handle_identifier();
    }
    else if (peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString)
    {
        create_node_current_token();

        peek_next_token();

        return;
    }
    else if (peek_current_token().subtype == TermLparen)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_arith_expr();

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler("expected: )");
            return NULL;
        }
        else
        {
            create_node_current_token();
            peek_next_token();
            return;
        }
    }
    else if (peek_current_token().subtype == TermLbracket)
    {
        return only_syntax_check_handle_array();
    }
    else
    {
        error_handler("expected Atom");
        return NULL;
    }
}

void only_syntax_check_handle_array()
{
    if (peek_current_token().subtype == TermLbracket)
    {
        create_node_current_token();

        peek_next_token();
        only_syntax_check_handle_items_array();

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("expected: []");
            return NULL;
        }
        else
        {
            create_node_current_token();
            peek_next_token();

            return;
        }
    }
    else
    {
        error_handler("expected: ]");
        return NULL;
    }
}

void only_syntax_check_handle_items_array()
{
    if (peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        only_syntax_check_handle_atom();

        only_syntax_check_handle_items_array_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_items_array_tail()
{
    if (peek_current_token().subtype == TermComma)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_atom();

        only_syntax_check_handle_items_array_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_expr_bool()
{
    return only_syntax_check_handle_expr_bool_or();
}

void only_syntax_check_handle_expr_bool_or()
{
    only_syntax_check_handle_expr_bool_and();

    only_syntax_check_handle_expr_bool_or_rest();

    return;
}

void only_syntax_check_handle_expr_bool_or_rest()
{
    if (peek_current_token().subtype == TermOr)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_expr_bool_and();

        only_syntax_check_handle_expr_bool_and_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_expr_bool_and()
{
    only_syntax_check_handle_expr_bool_not();

    only_syntax_check_handle_expr_bool_and_tail();

    return;
}

void only_syntax_check_handle_expr_bool_and_tail()
{
    if (peek_current_token().subtype == TermAnd)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_expr_bool_not();

        only_syntax_check_handle_expr_bool_and_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_expr_bool_not()
{
    if (peek_current_token().subtype == TermNot)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_expr_bool_not();
        return;
    }
    else
    {
        return only_syntax_check_handle_expr_bool_rel();
    }
}

void only_syntax_check_handle_expr_bool_rel()
{
    only_syntax_check_handle_arith_expr();

    only_syntax_check_handle_expr_bool_rel_tail();

    return;
}

void only_syntax_check_handle_expr_bool_rel_tail()
{
    if (peek_current_token().subtype == TermLe || peek_current_token().subtype == TermGe || peek_current_token().subtype == TermGt || peek_current_token().subtype == TermLt || peek_current_token().subtype == TermNe || peek_current_token().subtype == TermEq)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_arith_expr();

        return;
    }

    return;
}

void only_syntax_check_handle_expr_bool_rel_factor()
{
    return only_syntax_check_handle_arith_expr();
}

void only_syntax_check_handle_while_statement()
{
    if (peek_current_token().subtype == TermWhile)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_statement_structure();

        return;
    }

    return;
}

void only_syntax_check_handle_if_statement()
{
    if (peek_current_token().subtype == TermIf)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_statement_structure();
        only_syntax_check_handle_else_if_statement_list();

        only_syntax_check_handle_else_statement();

        return;
    }

    return;
}

void only_syntax_check_handle_else_if_statement_list()
{
    return only_syntax_check_handle_elseif_statement_tail();
}

void only_syntax_check_handle_else_if_statement()
{
    if (peek_current_token().subtype == TermElseIf)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_statement_structure();

        return;
    }

    return;
}

void only_syntax_check_handle_elseif_statement_tail()
{
    if (peek_current_token().subtype == TermElseIf)
    {
        only_syntax_check_handle_else_if_statement();

        only_syntax_check_handle_elseif_statement_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_else_statement()
{
    if (peek_current_token().subtype == TermElse)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_compound_statement();

        return;
    }

    return;
}

void only_syntax_check_handle_statement_structure()
{
    if (peek_current_token().subtype == TermLparen)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_expr_bool();

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler("expected:)");
            return NULL;
        }
        else
        {
            create_node_current_token();

            peek_next_token();

            only_syntax_check_handle_compound_statement();

            return;
        }
    }
    else
    {
        error_handler("expected :(");
        return NULL;
    }
}

void only_syntax_check_handle_for_statement()
{
    if (peek_current_token().subtype == TermFor)
    {
        create_node_current_token();

        peek_next_token();

        if (peek_current_token().subtype != TermLparen)
        {
            error_handler("expected: ()");
            return NULL;
        }
        else
        {
            create_node_current_token();

            peek_next_token();

            only_syntax_check_handle_assign_expr_list();

            if (peek_current_token().subtype != TermSemi)
            {
                error_handler("expected: ; 2");
                return NULL;
            }
            else
            {
                create_node_current_token();

                peek_next_token();
                only_syntax_check_handle_for_expr_bool();
                if (peek_current_token().subtype != TermSemi)
                {
                    error_handler("expected: ; 3");
                    return NULL;
                }
                else
                {
                    create_node_current_token();

                    peek_next_token();

                    only_syntax_check_handle_assign_expr_list();

                    if (peek_current_token().subtype != TermRparen)
                    {
                        error_handler("expected: )");
                        return NULL;
                    }
                    else
                    {
                        create_node_current_token();

                        peek_next_token();
                        only_syntax_check_handle_compound_statement();

                        return;
                    }
                }
            }
        }
    }

    return;
}

void only_syntax_check_handle_for_expr_bool()
{
    if (peek_current_token().subtype == TermNot || peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        return only_syntax_check_handle_expr_bool();
    }

    return;
}

void only_syntax_check_handle_assign_expr_list()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        only_syntax_check_handle_reassign_expr();

        only_syntax_check_handle_assign_expr_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_assign_expr_tail()
{
    if (peek_current_token().subtype == TermComma)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_reassign_expr();

        only_syntax_check_handle_assign_expr_tail();

        return;
    }

    return;
}

void only_syntax_check_handle_identifier()
{
    if (peek_current_token().subtype == TermIdentifier)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_identifier_array();

        return;
    }
    else
    {
        error_handler("expected id");
        return NULL;
    }
}

void only_syntax_check_handle_identifier_array()
{
    if (peek_current_token().subtype == TermLbracket)
    {
        create_node_current_token();

        peek_next_token();

        only_syntax_check_handle_arith_expr();

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("expected arrauyyp");
            return NULL;
        }
        else
        {
            create_node_current_token();

            peek_next_token();

            return;
        }
    }

    return;
}
