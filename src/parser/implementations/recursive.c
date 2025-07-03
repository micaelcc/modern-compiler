#include "recursive.h"
extern Token *TOKENS;
ParseTree *handle_program()
{
    ParseTree *node = create_parse_tree_node(NonTermProgram, (Token){
                                                                 TOKEN_UNK,
                                                                 -1,
                                                                 NULL});
    ParseTree *stmt_list = handle_statement_list();
    if (!stmt_list)
    {
        return node;
    }
    add_child_back(node, stmt_list);

    return node;
}

ParseTree *handle_statement_list()
{

    ParseTree *node = create_parse_tree_node(NonTermStatementList, (Token){
                                                                       TOKEN_UNK,
                                                                       -1,
                                                                       NULL});
    ParseTree *stmt = handle_statement();
    if (!stmt)
    {
        return node;
    }
    add_child_back(node, stmt);
    ParseTree *tail = handle_statement_list_tail();
    if (tail)
        add_child_back(node, tail);

    return node;
}

ParseTree *handle_statement_list_tail()
{

    Token tok = peek_current_token();
    if (tok.subtype == TermDefVar || tok.subtype == TermIdentifier || tok.subtype == TermWhile ||
        tok.subtype == TermFor || tok.subtype == TermIf)
    {
        ParseTree *node = create_parse_tree_node(NonTermStatementListTail, (Token){
                                                                               TOKEN_UNK,
                                                                               -1,
                                                                               NULL});
        add_child_back(node, handle_statement());
        add_child_back(node, handle_statement_list_tail());

        return node;
    }
    return NULL;
}

ParseTree *handle_statement()
{

    Token tok = peek_current_token();
    if (tok.subtype == TermIf)
        return handle_if_statement();
    if (tok.subtype == TermWhile)
        return handle_while_statement();
    if (tok.subtype == TermFor)
        return handle_for_statement();

    ParseTree *node = create_parse_tree_node(NonTermStatement, (Token){
                                                                   TOKEN_UNK,
                                                                   -1,
                                                                   NULL});
    ParseTree *expr = handle_expr();
    if (!expr)
    {
        free_parse_tree(node);
        return NULL;
    }
    add_child_back(node, expr);

    if (peek_current_token().type == TOKEN_SEMICOLON)
    {
        Token semi = peek_current_token();
        peek_next_token();
        add_child_back(node, create_parse_tree_node(TermSemi, semi));

        return node;
    }
    else
    {
        error_handler("Expected: ;");
        free_parse_tree(node);
        return NULL;
    }
}

ParseTree *handle_if_statement()
{

    Token tok = peek_current_token();
    peek_next_token();
    ParseTree *node = create_parse_tree_node(NonTermIfStatement, (Token){
                                                                     TOKEN_UNK,
                                                                     -1,
                                                                     NULL});
    add_child_back(node, create_parse_tree_node(TermIf, tok));
    add_child_back(node, handle_statement_structure());
    ParseTree *elseifs = handle_else_if_statement_list();
    if (elseifs)
        add_child_back(node, elseifs);
    ParseTree *els = handle_else_statement();
    if (els)
        add_child_back(node, els);

    return node;
}

ParseTree *handle_else_if_statement_list()
{
    return handle_elseif_statement_tail();
}

ParseTree *handle_else_if_statement()
{

    Token tok = peek_current_token();
    peek_next_token();
    ParseTree *node = create_parse_tree_node(NonTermElseifStatement, (Token){
                                                                         TOKEN_UNK,
                                                                         -1,
                                                                         NULL});
    add_child_back(node, create_parse_tree_node(TermElseIf, tok));
    add_child_back(node, handle_statement_structure());

    return node;
}

ParseTree *handle_elseif_statement_tail()
{

    Token tok = peek_current_token();
    if (tok.subtype == TermElseIf)
    {
        ParseTree *node = create_parse_tree_node(NonTermElseifStatementTail, (Token){
                                                                                 TOKEN_UNK,
                                                                                 -1,
                                                                                 NULL});
        add_child_back(node, handle_else_if_statement());
        ParseTree *tail = handle_elseif_statement_tail();
        if (tail)
            add_child_back(node, tail);

        return node;
    }

    return NULL;
}

ParseTree *handle_else_statement()
{

    Token tok = peek_current_token();
    if (tok.subtype != TermElse)
        return NULL;
    peek_next_token();
    ParseTree *node = create_parse_tree_node(NonTermElseStatement, (Token){
                                                                       TOKEN_UNK,
                                                                       -1,
                                                                       NULL});
    add_child_back(node, create_parse_tree_node(TermElse, tok));
    add_child_back(node, handle_compound_statement());

    return node;
}

ParseTree *handle_statement_structure()
{

    Token lparen = peek_current_token();
    if (lparen.subtype != TermLparen)
    {
        error_handler("Expected: (");
        return NULL;
    }
    peek_next_token();

    ParseTree *node = create_parse_tree_node(NonTermStatementStructure, (Token){
                                                                            TOKEN_UNK,
                                                                            -1,
                                                                            NULL});
    add_child_back(node, create_parse_tree_node(TermLparen, lparen));
    add_child_back(node, handle_expr_bool());

    Token rparen = peek_current_token();
    if (rparen.subtype != TermRparen)
    {
        error_handler("Expected: )");
        free_parse_tree(node);
        return NULL;
    }
    peek_next_token();
    add_child_back(node, create_parse_tree_node(TermRparen, rparen));

    add_child_back(node, handle_compound_statement());

    return node;
}

ParseTree *handle_arith_expr()
{

    ParseTree *node = create_parse_tree_node(NonTermArithExpr, peek_current_token());
    add_child_back(node, handle_term());
    add_child_back(node, handle_arith_expr_tail());

    return node;
}

ParseTree *handle_reassign()
{

    ParseTree *node = create_parse_tree_node(NonTermReassignExpr, peek_current_token());

    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_identifier());

        if (peek_current_token().subtype != TermAssign)
        {
            error_handler((char *)"Expected: =");
        }
        else
        {
            add_child_back(node, create_parse_tree_node(TermAssign, peek_current_token()));
            peek_next_token();
            add_child_back(node, handle_arith_expr());
        }
    }
    else
    {
        error_handler((char *)"Expected: Identifier");
    }

    return node;
}

ParseTree *handle_expr_bool()
{
    return handle_expr_bool_or();
}

ParseTree *handle_compound_statement()
{

    ParseTree *node = create_parse_tree_node(NonTermCompoundStatement, peek_current_token());

    if (peek_current_token().subtype == TermLbrace)
    {
        add_child_back(node, create_parse_tree_node(TermLbrace, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_statement_list());

        if (peek_current_token().subtype != TermRbrace)
        {
            err_expected(RBRACE, peek_current_token().value);
            return node;
        }
        else
        {
            add_child_back(node, create_parse_tree_node(TermRbrace, peek_current_token()));
            peek_next_token();
        }
    }
    else
    {
        err_expected(LBRACE, peek_current_token().value);
    }

    return node;
}

ParseTree *handle_expr()
{

    ParseTree *node = create_parse_tree_node(NonTermExpr, peek_current_token());

    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_identifier());
        add_child_back(node, handle_reassign_expr_tail());
    }
    else if (peek_current_token().subtype == TermDefVar)
    {
        add_child_back(node, handle_decl_expr());
    }
    else
    {
        error_handler((char *)"Expected: let | id");
    }

    return node;
}

ParseTree *handle_while_statement()
{

    ParseTree *node = create_parse_tree_node(NonTermWhileStatement, peek_current_token());

    if (peek_current_token().subtype == TermWhile)
    {
        add_child_back(node, create_parse_tree_node(TermWhile, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_statement_structure());
    }

    return node;
}

ParseTree *handle_arith_expr_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermArithExprTail, peek_current_token());
    add_child_back(node, handle_arith_expr_tail_rest());

    return node;
}

ParseTree *handle_expr_bool_or()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolOr, peek_current_token());
    add_child_back(node, handle_expr_bool_and());
    add_child_back(node, handle_expr_bool_or_rest());

    return node;
}

ParseTree *handle_reassign_expr_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermReassignExprTail, peek_current_token());

    if (peek_current_token().subtype == TermAssign)
    {
        add_child_back(node, create_parse_tree_node(TermAssign, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_arith_expr());
    }
    else
    {
        add_child_back(node, handle_arith_expr_tail());
    }

    return node;
}

ParseTree *handle_decl_expr()
{

    ParseTree *node = create_parse_tree_node(NonTermDeclExpr, peek_current_token());

    if (peek_current_token().subtype == TermDefVar)
    {
        add_child_back(node, create_parse_tree_node(TermDefVar, peek_current_token()));
        peek_next_token();

        if (peek_current_token().subtype != TermIdentifier)
        {
            error_handler((char *)"Expected: Identifier");
        }
        else
        {
            add_child_back(node, handle_identifier());
            add_child_back(node, handle_decl_expr_assign());
        }
    }
    else
    {
        error_handler((char *)"Expected: let");
    }

    return node;
}

ParseTree *handle_reassign_expr()
{
    Token curr_token = peek_current_token();
    ParseTree *node = create_parse_tree_node(NonTermReassignExpr, curr_token);

    if (curr_token.subtype == TermIdentifier)
    {
        add_child_back(node, handle_identifier());

        if (peek_current_token().subtype != TermAssign)
        {
            err_expected(ASSIGN, peek_current_token().value);
        }
        else
        {
            add_child_back(node, create_parse_tree_node(TermAssign, peek_current_token()));
            peek_next_token();
            add_child_back(node, handle_arith_expr());
        }
    }
    else
    {
        err_expected(ID, peek_current_token().value);
    }

    return node;
}

ParseTree *handle_term()
{

    ParseTree *node = create_parse_tree_node(NonTermTerm, peek_current_token());
    add_child_back(node, handle_factor());
    add_child_back(node, handle_term_list());

    return node;
}

ParseTree *handle_term_list()
{

    ParseTree *node = create_parse_tree_node(NonTermTermList, peek_current_token());
    add_child_back(node, handle_term_list_tail());

    return node;
}

ParseTree *handle_term_list_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermTermListTail, peek_current_token());

    if (peek_current_token().subtype == TermMul || peek_current_token().subtype == TermDiv)
    {
        int op = peek_current_token().subtype;
        add_child_back(node, create_parse_tree_node(op, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_factor());
        add_child_back(node, handle_term_list_tail());
    }

    return node;
}

ParseTree *handle_expr_bool_and()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolAnd, peek_current_token());
    add_child_back(node, handle_expr_bool_not());
    add_child_back(node, handle_expr_bool_and_tail());

    return node;
}

ParseTree *handle_expr_bool_or_rest()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolOrRest, peek_current_token());

    if (peek_current_token().subtype == TermOr)
    {
        add_child_back(node, create_parse_tree_node(TermOr, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_expr_bool_and());
        add_child_back(node, handle_expr_bool_or_rest());
    }

    return node;
}

ParseTree *handle_decl_expr_assign()
{

    ParseTree *node = create_parse_tree_node(NonTermDeclExprAssign, peek_current_token());

    if (peek_current_token().subtype == TermAssign)
    {
        add_child_back(node, create_parse_tree_node(TermAssign, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_arith_expr());
    }

    return node;
}

ParseTree *handle_expr_bool_not()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolNot, peek_current_token());

    if (peek_current_token().subtype == TermNot)
    {
        add_child_back(node, create_parse_tree_node(TermNot, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_expr_bool_not());
    }
    else
    {
        add_child_back(node, handle_expr_bool_rel());
    }

    return node;
}

ParseTree *handle_expr_bool_rel()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolRel, peek_current_token());
    add_child_back(node, handle_arith_expr());
    add_child_back(node, handle_expr_bool_rel_tail());

    return node;
}

ParseTree *handle_expr_bool_rel_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolRelTail, peek_current_token());

    if (peek_current_token().subtype == TermLe || peek_current_token().subtype == TermGe ||
        peek_current_token().subtype == TermGt || peek_current_token().subtype == TermLt ||
        peek_current_token().subtype == TermNe || peek_current_token().subtype == TermEq)
    {
        int op = peek_current_token().subtype;
        add_child_back(node, create_parse_tree_node(op, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_arith_expr());
    }

    return node;
}

ParseTree *handle_expr_bool_rel_factor()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolRelFactor, peek_current_token());
    add_child_back(node, handle_arith_expr());

    return node;
}

ParseTree *handle_factor()
{

    ParseTree *node = create_parse_tree_node(NonTermFactor, peek_current_token());
    add_child_back(node, handle_factor_rest());

    return node;
}

ParseTree *handle_factor_rest()
{

    ParseTree *node = create_parse_tree_node(NonTermFactorRest, peek_current_token());

    if (peek_current_token().subtype == TermPlus || peek_current_token().subtype == TermMinus)
    {
        int op = peek_current_token().subtype;
        add_child_back(node, create_parse_tree_node(op, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_pow());
    }
    else
    {
        add_child_back(node, handle_pow());
    }

    return node;
}

ParseTree *handle_pow()
{

    ParseTree *node = create_parse_tree_node(NonTermPow, peek_current_token());
    add_child_back(node, handle_atom());
    add_child_back(node, handle_pow_rest());

    return node;
}

ParseTree *handle_pow_rest()
{

    ParseTree *node = create_parse_tree_node(NonTermPowRest, peek_current_token());

    if (peek_current_token().subtype == TermPow)
    {
        add_child_back(node, create_parse_tree_node(TermPow, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_pow());
    }

    return node;
}

ParseTree *handle_atom()
{

    ParseTree *node = create_parse_tree_node(NonTermAtom, peek_current_token());

    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_identifier());
    }
    else if (peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString)
    {
        add_child_back(node, create_parse_tree_node(peek_current_token().subtype, peek_current_token()));
        peek_next_token();
    }
    else if (peek_current_token().subtype == TermLparen)
    {
        add_child_back(node, create_parse_tree_node(TermLparen, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_arith_expr());

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler((char *)"Expected: )");
        }
        else
        {
            add_child_back(node, create_parse_tree_node(TermRparen, peek_current_token()));
            peek_next_token();
        }
    }
    else if (peek_current_token().subtype == TermLbracket)
    {
        add_child_back(node, handle_array());
    }
    else
    {
        error_handler((char *)"Expected: Atom");
    }

    return node;
}

ParseTree *handle_array()
{

    ParseTree *node = create_parse_tree_node(NonTermArray, peek_current_token());

    if (peek_current_token().subtype == TermLbracket)
    {
        add_child_back(node, create_parse_tree_node(TermLbracket, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_items_array());

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler((char *)"Expected: ]");
        }
        else
        {
            add_child_back(node, create_parse_tree_node(TermRbracket, peek_current_token()));
            peek_next_token();
        }
    }
    else
    {
        error_handler((char *)"Expected: [");
    }

    return node;
}

ParseTree *handle_items_array()
{

    ParseTree *node = create_parse_tree_node(NonTermItemsArray, peek_current_token());

    if (peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen ||
        peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat ||
        peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_atom());
        add_child_back(node, handle_items_array_tail());
    }

    return node;
}

ParseTree *handle_items_array_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermItemsArrayTail, peek_current_token());

    if (peek_current_token().subtype == TermComma)
    {
        add_child_back(node, create_parse_tree_node(TermComma, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_atom());
        add_child_back(node, handle_items_array_tail());
    }

    return node;
}
ParseTree *handle_for_statement()
{

    ParseTree *node = create_parse_tree_node(NonTermForStatement, peek_current_token());

    if (peek_current_token().subtype == TermFor)
    {
        add_child_back(node, create_parse_tree_node(TermFor, peek_current_token()));
        peek_next_token();

        if (peek_current_token().subtype != TermLparen)
        {
            error_handler((char *)"Expected: (");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermLparen, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_assign_expr_list());

        if (peek_current_token().subtype != TermSemi)
        {
            error_handler((char *)"Expected: ;");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermSemi, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_for_expr_bool());

        if (peek_current_token().subtype != TermSemi)
        {
            error_handler((char *)"Expected: ;");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermSemi, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_assign_expr_list());

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler((char *)"Expected: )");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermRparen, peek_current_token()));
        peek_next_token();

        add_child_back(node, handle_compound_statement());
    }

    return node;
}

ParseTree *handle_for_expr_bool()
{

    ParseTree *node = create_parse_tree_node(NonTermForExprBool, peek_current_token());

    if (peek_current_token().subtype == TermNot || peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen ||
        peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString ||
        peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_expr_bool());
    }

    return node;
}

ParseTree *handle_arith_expr_tail_rest()
{

    ParseTree *node = create_parse_tree_node(NonTermArithExprTailRest, peek_current_token());

    if (peek_current_token().subtype == TermPlus || peek_current_token().subtype == TermMinus)
    {
        int op = peek_current_token().subtype;
        add_child_back(node, create_parse_tree_node(op, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_term());
        add_child_back(node, handle_arith_expr_tail_rest());
    }

    return node;
}

ParseTree *handle_expr_bool_and_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermExprBoolAndTail, peek_current_token());

    if (peek_current_token().subtype == TermAnd)
    {
        add_child_back(node, create_parse_tree_node(TermAnd, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_expr_bool_not());
        add_child_back(node, handle_expr_bool_and_tail());
    }

    return node;
}

ParseTree *handle_assign_expr_list()
{

    ParseTree *node = create_parse_tree_node(NonTermAssignExprList, peek_current_token());

    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, handle_reassign_expr());
        add_child_back(node, handle_assign_expr_tail());
    }

    return node;
}

ParseTree *handle_assign_expr_tail()
{

    ParseTree *node = create_parse_tree_node(NonTermAssignExprTail, peek_current_token());

    if (peek_current_token().subtype == TermComma)
    {
        add_child_back(node, create_parse_tree_node(TermComma, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_reassign_expr());
        add_child_back(node, handle_assign_expr_tail());
    }

    return node;
}

ParseTree *handle_identifier()
{

    ParseTree *node = create_parse_tree_node(NonTermIdentifier, peek_current_token());

    if (peek_current_token().subtype == TermIdentifier)
    {
        add_child_back(node, create_parse_tree_node(TermIdentifier, peek_current_token()));
        peek_next_token();
        add_child_back(node, handle_identifier_array());
    }
    else
    {
        error_handler((char *)"Expected: id");
    }

    return node;
}

ParseTree *handle_identifier_array()
{

    ParseTree *node = create_parse_tree_node(NonTermIdentifierArray, peek_current_token());

    if (peek_current_token().subtype == TermLbracket)
    {
        Token open = peek_current_token();
        peek_next_token();

        ParseTree *expr = handle_arith_expr();
        if (!expr)
        {
            error_handler("Expected expression inside []");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermLbracket, open));
        add_child_back(node, expr);

        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler("Expected: ]");
            return node;
        }

        add_child_back(node, create_parse_tree_node(TermRbracket, peek_current_token()));
        peek_next_token();
    }

    return node;
}

/* ######## FOR TESTING PURPOSES ########
 *
 * Parser without parse tree generation
 */
void handle_program_syntax_only()
{

    handle_statement_list_syntax_only();
}

void handle_statement_list_syntax_only()
{

    handle_statement_syntax_only();
    handle_statement_list_tail_syntax_only();
}

void handle_statement_syntax_only()
{

    Token curr_tok = peek_current_token();
    if (curr_tok.subtype == TermIf)
    {
        return handle_if_statement_syntax_only();
    }
    else if (curr_tok.subtype == TermWhile)
    {
        return handle_while_statement_syntax_only();
    }
    else if (curr_tok.subtype == TermFor)
    {
        return handle_for_statement_syntax_only();
    }
    else if (curr_tok.type == TOKEN_AND || curr_tok.type == TOKEN_ASSIGN || curr_tok.type == TOKEN_COMMA || curr_tok.type == TOKEN_DIV || curr_tok.type == TOKEN_EQ || curr_tok.type == TOKEN_GE || curr_tok.type == TOKEN_GT || curr_tok.type == TOKEN_LBRACE || curr_tok.type == TOKEN_LE || curr_tok.type == TOKEN_LSQUARE || curr_tok.type == TOKEN_LT || curr_tok.type == TOKEN_MUL || curr_tok.type == TOKEN_NE || curr_tok.type == TOKEN_NOT || curr_tok.type == TOKEN_OR || curr_tok.type == TOKEN_RSQUARE || curr_tok.type == TOKEN_RPAREN || curr_tok.type == TOKEN_RBRACE || curr_tok.type == TOKEN_POW)
    {
        error_handler("Expected: Expr | If | While | For");
        return;
    }
    else
    {
        handle_expr_syntax_only();

        if (peek_current_token().type == TOKEN_SEMICOLON)
        {
            peek_next_token();
            return;
        }
        else
        {
            error_handler("Expected: ;");
            return;
        }
    }
}

void handle_statement_list_tail_syntax_only()
{

    if (peek_current_token().subtype == TermDefVar || peek_current_token().subtype == TermIdentifier || peek_current_token().subtype == TermWhile || peek_current_token().subtype == TermFor || peek_current_token().subtype == TermIf || peek_current_token().subtype == TermDefVar)
    {
        handle_statement_syntax_only();
        handle_statement_list_tail_syntax_only();

        return;
    }
}

void handle_compound_statement_syntax_only()
{

    if (peek_current_token().subtype == TermLbrace)
    {
        peek_next_token();

        handle_statement_list_syntax_only();

        if (peek_current_token().subtype != TermRbrace)
        {
            error_handler("Expected: }");
            return;
        }
        else
        {
            peek_next_token();

            return;
        }
    }
    else
    {
        error_handler("Expected: {");
    }
}

void handle_expr_syntax_only()
{

    if (peek_current_token().subtype == TermIdentifier)
    {
        handle_identifier_syntax_only();

        handle_reassign_expr_tail_syntax_only();
    }
    else if (peek_current_token().subtype == TermDefVar)
    {
        return handle_decl_expr_syntax_only();
    }
    else
    {
        error_handler((char *)"Expected: let | id");
    }
}

void handle_reassign_expr_syntax_only()
{

    if (peek_current_token().subtype == TermIdentifier)
    {
        handle_identifier_syntax_only();
        if (peek_current_token().subtype != TermAssign)
        {
            error_handler((char *)"Expected: =");
        }
        else
        {
            peek_next_token();
            handle_arith_expr_syntax_only();
        }
    }
    else
    {
        error_handler((char *)"Expected: Identifier");
    }
}

void handle_decl_expr_syntax_only()
{

    if (peek_current_token().subtype == TermDefVar)
    {
        peek_next_token();
        if (peek_current_token().subtype != TermIdentifier)
        {
            error_handler((char *)"Expected: Identifier");
        }
        else
        {
            handle_identifier_syntax_only();
            handle_decl_expr_assign_syntax_only();
        }
    }
    else
    {
        error_handler((char *)"Expected: let");
    }
}

void handle_decl_expr_assign_syntax_only()
{

    if (peek_current_token().subtype == TermAssign)
    {
        peek_next_token();
        handle_arith_expr_syntax_only();
    }
}

void handle_reassign_expr_tail_syntax_only()
{

    if (peek_current_token().subtype == TermAssign)
    {
        peek_next_token();
        handle_arith_expr_syntax_only();
    }
    else
    {
        return handle_arith_expr_tail_syntax_only();
    }
}

void handle_arith_expr_syntax_only()
{

    handle_term_syntax_only();
    handle_arith_expr_tail_syntax_only();
}

void handle_arith_expr_tail_syntax_only()
{

    return handle_arith_expr_tail_rest_syntax_only();
}

void handle_arith_expr_tail_rest_syntax_only()
{

    if (peek_current_token().subtype == TermPlus)
    {
        peek_next_token();
        handle_term_syntax_only();
        handle_arith_expr_tail_rest_syntax_only();
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        peek_next_token();
        handle_term_syntax_only();
        handle_arith_expr_tail_rest_syntax_only();
    }
}

void handle_term_syntax_only()
{

    handle_factor_syntax_only();
    handle_term_list_syntax_only();
}

void handle_term_list_syntax_only()
{

    return handle_term_list_tail_syntax_only();
}

void handle_term_list_tail_syntax_only()
{

    if (peek_current_token().subtype == TermMul)
    {
        peek_next_token();
        handle_factor_syntax_only();
        handle_term_list_tail_syntax_only();
    }
    else if (peek_current_token().subtype == TermDiv)
    {
        peek_next_token();
        handle_factor_syntax_only();
        handle_term_list_tail_syntax_only();
    }
}

void handle_factor_syntax_only()
{

    return handle_factor_rest_syntax_only();
}

void handle_factor_rest_syntax_only()
{

    if (peek_current_token().subtype == TermPlus)
    {
        peek_next_token();
        handle_pow_syntax_only();
    }
    else if (peek_current_token().subtype == TermMinus)
    {
        peek_next_token();
        handle_pow_syntax_only();
    }
    else
    {
        return handle_pow_syntax_only();
    }
}

void handle_pow_syntax_only()
{

    handle_atom_syntax_only();
    handle_pow_rest_syntax_only();
}

void handle_pow_rest_syntax_only()
{

    if (peek_current_token().subtype == TermPow)
    {
        peek_next_token();
        handle_pow_syntax_only();
    }
}

void handle_atom_syntax_only()
{

    if (peek_current_token().subtype == TermIdentifier)
    {
        handle_identifier_syntax_only();
    }
    else if (peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString)
    {
        peek_next_token();
    }
    else if (peek_current_token().subtype == TermLparen)
    {
        peek_next_token();
        handle_arith_expr_syntax_only();

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler((char *)"Expected: )");
        }
        else
        {
            peek_next_token();
        }
    }
    else if (peek_current_token().subtype == TermLbracket)
    {
        handle_array_syntax_only();
    }
    else
    {
        error_handler((char *)"Expected: Atom");
    }
}

void handle_array_syntax_only()
{

    if (peek_current_token().subtype == TermLbracket)
    {
        peek_next_token();
        handle_items_array_syntax_only();
        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler((char *)"Expected: ]");
        }
        else
        {
            peek_next_token();
        }
    }
    else
    {
        error_handler((char *)"Expected: [");
    }
}

void handle_items_array_syntax_only()
{

    if (peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        handle_atom_syntax_only();
        handle_items_array_tail_syntax_only();
    }
}

void handle_items_array_tail_syntax_only()
{

    if (peek_current_token().subtype == TermComma)
    {
        peek_next_token();
        handle_atom_syntax_only();
        handle_items_array_tail_syntax_only();
    }
}

void handle_expr_bool_syntax_only()
{

    return handle_expr_bool_or_syntax_only();
}

void handle_expr_bool_or_syntax_only()
{

    handle_expr_bool_and_syntax_only();
    handle_expr_bool_or_rest_syntax_only();
}

void handle_expr_bool_or_rest_syntax_only()
{

    if (peek_current_token().subtype == TermOr)
    {
        peek_next_token();
        handle_expr_bool_and_syntax_only();
        handle_expr_bool_or_rest_syntax_only();
    }
}

void handle_expr_bool_and_syntax_only()
{

    handle_expr_bool_not_syntax_only();
    handle_expr_bool_and_tail_syntax_only();
}

void handle_expr_bool_and_tail_syntax_only()
{

    if (peek_current_token().subtype == TermAnd)
    {
        peek_next_token();
        handle_expr_bool_not_syntax_only();
        handle_expr_bool_and_tail_syntax_only();
    }
}

void handle_expr_bool_not_syntax_only()
{

    if (peek_current_token().subtype == TermNot)
    {
        peek_next_token();
        handle_expr_bool_not_syntax_only();
    }
    else
    {
        handle_expr_bool_rel_syntax_only();
    }
}

void handle_expr_bool_rel_syntax_only()
{

    handle_arith_expr_syntax_only();
    handle_expr_bool_rel_tail_syntax_only();
}

void handle_expr_bool_rel_tail_syntax_only()
{

    if (peek_current_token().subtype == TermLe || peek_current_token().subtype == TermGe || peek_current_token().subtype == TermGt || peek_current_token().subtype == TermLt || peek_current_token().subtype == TermNe || peek_current_token().subtype == TermEq)
    {
        peek_next_token();
        handle_arith_expr_syntax_only();
    }
}

void handle_expr_bool_rel_factor_syntax_only()
{

    handle_arith_expr_syntax_only();
}

void handle_while_statement_syntax_only()
{

    if (peek_current_token().subtype == TermWhile)
    {
        peek_next_token();
        handle_statement_structure_syntax_only();
    }
}

void handle_if_statement_syntax_only()
{

    if (peek_current_token().subtype == TermIf)
    {
        peek_next_token();
        handle_statement_structure_syntax_only();
        handle_else_if_statement_list_syntax_only();
        handle_else_statement_syntax_only();
    }
}

void handle_else_if_statement_list_syntax_only()
{

    return handle_elseif_statement_tail_syntax_only();
}

void handle_else_if_statement_syntax_only()
{

    if (peek_current_token().subtype == TermElseIf)
    {
        peek_next_token();
        handle_statement_structure_syntax_only();
    }
}

void handle_elseif_statement_tail_syntax_only()
{

    if (peek_current_token().subtype == TermElseIf)
    {
        handle_else_if_statement_syntax_only();
        handle_elseif_statement_tail_syntax_only();
    }
}

void handle_else_statement_syntax_only()
{

    if (peek_current_token().subtype == TermElse)
    {
        peek_next_token();
        handle_compound_statement_syntax_only();
    }
}

void handle_statement_structure_syntax_only()
{

    if (peek_current_token().subtype == TermLparen)
    {
        peek_next_token();
        handle_expr_bool_syntax_only();

        if (peek_current_token().subtype != TermRparen)
        {
            error_handler((char *)"Expected: )");
        }
        else
        {
            peek_next_token();
            handle_compound_statement_syntax_only();
        }
    }
    else
    {
        error_handler((char *)"Expected: (");
    }
}

void handle_for_statement_syntax_only()
{

    if (peek_current_token().subtype == TermFor)
    {
        peek_next_token();
        if (peek_current_token().subtype != TermLparen)
        {
            error_handler((char *)"Expected: (");
        }
        else
        {
            peek_next_token();
            handle_assign_expr_list_syntax_only();
            if (peek_current_token().subtype != TermSemi)
            {
                error_handler((char *)"Expected: ;");
            }
            else
            {
                peek_next_token();
                handle_for_expr_bool_syntax_only();
                if (peek_current_token().subtype != TermSemi)
                {
                    error_handler((char *)"Expected: ;");
                }
                else
                {
                    peek_next_token();
                    handle_assign_expr_list_syntax_only();

                    if (peek_current_token().subtype != TermRparen)
                    {
                        error_handler((char *)"Expected: )");
                    }
                    else
                    {
                        peek_next_token();
                        handle_compound_statement_syntax_only();
                    }
                }
            }
        }
    }
}

void handle_for_expr_bool_syntax_only()
{

    if (peek_current_token().subtype == TermNot || peek_current_token().subtype == TermLbracket || peek_current_token().subtype == TermLparen || peek_current_token().subtype == TermInteger || peek_current_token().subtype == TermFloat || peek_current_token().subtype == TermString || peek_current_token().subtype == TermIdentifier)
    {
        handle_expr_bool_syntax_only();
    }
}

void handle_assign_expr_list_syntax_only()
{

    if (peek_current_token().subtype == TermIdentifier)
    {
        handle_reassign_expr_syntax_only();
        handle_assign_expr_tail_syntax_only();
    }
}

void handle_assign_expr_tail_syntax_only()
{

    if (peek_current_token().subtype == TermComma)
    {
        peek_next_token();
        handle_reassign_expr_syntax_only();
        handle_assign_expr_tail_syntax_only();
    }
}

void handle_identifier_syntax_only()
{

    if (peek_current_token().subtype == TermIdentifier)
    {
        peek_next_token();
        handle_identifier_array_syntax_only();
    }
    else
    {
        error_handler((char *)"Expected: id");
    }
}

void handle_identifier_array_syntax_only()
{

    if (peek_current_token().subtype == TermLbracket)
    {
        peek_next_token();
        handle_arith_expr_syntax_only();
        if (peek_current_token().subtype != TermRbracket)
        {
            error_handler((char *)"Expected: ]");
        }
        else
        {
            peek_next_token();
        }
    }
}
