#include "non-recursive.h"

ASTNode *parser_non_recurs()
{
    ParseTable table = get_table();
    GQueue *stack = g_queue_new();
    GQueue *ast_stack = g_queue_new();
    GQueue *op_stack = g_queue_new();
    GQueue *block_stack = g_queue_new();

    g_queue_push_head(stack, GINT_TO_POINTER(NonTermProgram));

    ASTNode *root = create_node("Program", get_undef_token());
    ASTNode *current_parent = root;
    ASTNode *current_array = NULL;
    Token identifier_being_parsed = get_undef_token();
    IsParsingFlags is_parsing = {};

    while (!g_queue_is_empty(stack))
    {
        int top = GPOINTER_TO_INT(g_queue_pop_head(stack));
        Token curr_token = peek_current_token();

        if (top == TermEof && curr_token.type == TOKEN_EOF)
        {
            g_queue_free(stack);
            return root;
        }

        if (top >= NonTermProgram && top <= NonTermIdentifierArray)
        {
            int *prods = table[top][curr_token.subtype];

            if (prods != NULL)
            {
                int *p = prods;
                while (*p != -1)
                    p++;

                for (p--; p >= prods; p--)
                    if (*p != TermEpsilon)
                        g_queue_push_head(stack, GINT_TO_POINTER(*p));

                perform_ast_node(top, curr_token, ast_stack, op_stack);
            }
            else
            {
                error_expected_non_term(top);
                g_queue_free(stack);
                return NULL;
            }
        }
        else if (top == curr_token.subtype)
        {
            if (TermLparen == top)
                g_queue_push_head(op_stack, create_node(curr_token.value, curr_token));
            else if (TermAssign == top && !is_parsing._let)
                is_parsing._reassign = true;
            else if (TermIdentifier == top && !identifier_being_parsed.value)
                identifier_being_parsed = curr_token;
            else if (TermIf == top)
                is_parsing._if = true;
            else if (TermElse == top)
                is_parsing._else = true;
            else if (TermElseIf == top)
                is_parsing._elseif = true;
            else if (TermWhile == top)
                is_parsing._while = true;
            else if (TermFor == top)
            {
                is_parsing._for = true;
                is_parsing._for_initial_assignments = true;
            }
            else if (TermLbracket == top)
            {
                is_parsing._array = true;
                current_array = create_node("array", get_undef_token());
            }
            else if (TermDefVar == top)
            {
                is_parsing._let = true;
                identifier_being_parsed = peek_next_token_no_advance();
            }

            peek_next_token();
        }
        else

        {
            printf("Expected: %s, found: %s\n",
                   terminal_enum_to_str(top),
                   terminal_enum_to_str(curr_token.subtype));
            g_queue_free(stack);
            return NULL;
        }

        handle_ast(
            top,
            curr_token,
            &identifier_being_parsed,
            &is_parsing,
            ast_stack,
            op_stack,
            block_stack,
            root,
            current_array);
    }

    g_queue_free(stack);
    return NULL;
}

void handle_ast(
    int top_symbol,
    Token curr_tok,
    Token *identifier_being_parsed,
    IsParsingFlags *is_parsing,
    gpointer ast_stack,
    gpointer op_stack,
    gpointer block_stack,
    ASTNode *root,
    ASTNode *current_array)
{

    if ((top_symbol == NonTermArithExprTailRest || top_symbol == NonTermFactorRest || top_symbol == NonTermTermListTail || top_symbol == NonTermOpBool || top_symbol == NonTermExprBoolOrRest || top_symbol == NonTermExprBoolAndTail) && (curr_tok.subtype == TermSemi || curr_tok.subtype == TermRparen || curr_tok.subtype == TermRbrace))
    {
        while (!g_queue_is_empty(op_stack) && create_binop(ast_stack, op_stack))
            ;
    }
    else if (top_symbol == TermSemi && is_parsing->_let)
    {
        add_statement(root, create_let(identifier_being_parsed, current_array, ast_stack), block_stack, false);
        is_parsing->_let = false;
        is_parsing->_array = false;
    }
    else if (top_symbol == TermLbrace && is_parsing->_if)
    {
        add_statement(root, create_if_and_elseif(ast_stack, block_stack, IF), block_stack, true);
        is_parsing->_if = false;
    }
    else if (is_parsing->_else)
    {
        add_statement(root, create_else(ast_stack, block_stack), block_stack, true);
        is_parsing->_else = false;
    }
    else if (top_symbol == TermLbrace && is_parsing->_elseif)
    {
        add_statement(root, create_if_and_elseif(ast_stack, block_stack, ELSEIF), block_stack, true);
        is_parsing->_elseif = false;
    }
    else if (top_symbol == TermSemi && is_parsing->_reassign && !is_parsing->_for)
    {
        add_statement(root, create_reassign(identifier_being_parsed, current_array, ast_stack), block_stack, false);
        is_parsing->_reassign = false;
        is_parsing->_array = false;
    }
    else if (top_symbol == TermSemi && is_parsing->_for_condition)
    {
        update_node_for(root, 1, g_queue_pop_head(ast_stack));

        is_parsing->_for_condition = false;
        is_parsing->_for_final_assignments = true;
    }
    else if ((top_symbol == TermIdentifier || top_symbol == TermInteger || top_symbol == TermFloat || top_symbol == TermString) && is_parsing->_array)
    {
        add_child(current_array, create_node(curr_tok.value, curr_tok));
    }
    else if ((top_symbol == TermComma || top_symbol == TermSemi) && is_parsing->_for_initial_assignments)
    {
        update_node_for(root, 0, create_reassign(identifier_being_parsed, current_array, ast_stack));

        if (top_symbol == TermSemi)
        {
            is_parsing->_for_initial_assignments = false;
            is_parsing->_for_condition = true;
        }
    }
    else if ((top_symbol == TermComma || top_symbol == TermRparen) && is_parsing->_for_final_assignments)
    {
        update_node_for(root, 2, create_reassign(identifier_being_parsed, current_array, ast_stack));

        if (top_symbol == TermRparen)
        {
            is_parsing->_for = false;
            is_parsing->_for_final_assignments = false;
        }
    }
    else if (top_symbol == TermFor && is_parsing->_for)
    {
        add_statement(root, create_for(ast_stack, block_stack), block_stack, true);
    }
    else if (top_symbol == TermLbrace && is_parsing->_while)
    {
        add_statement(root, create_if_and_elseif(ast_stack, block_stack, WHILE), block_stack, true);
        is_parsing->_while = false;
    }
    else if (top_symbol == TermRbrace)
    {
        g_queue_pop_head(block_stack);
    }
}

void perform_ast_node(int top_symbol, Token curr_tok, gpointer ast_stack, gpointer op_stack)
{
    ASTNode *node = create_node(curr_tok.value, curr_tok);
    int subtype = curr_tok.subtype;

    if (top_symbol == NonTermIdentifier && subtype == TermIdentifier)
        g_queue_push_head(ast_stack, node);
    if (top_symbol == NonTermAtom && subtype == TermLparen)
        g_queue_push_head(op_stack, node);
    if (top_symbol == NonTermAtom && subtype == TermInteger)
        g_queue_push_head(ast_stack, node);
    if (
        ((top_symbol == NonTermArithExprTailRest || top_symbol == NonTermFactorRest) && (subtype == TermPlus || subtype == TermMinus)) ||
        (top_symbol == NonTermOpBool && (subtype == TermGe || subtype == TermLe || subtype == TermGt || subtype == TermLt || subtype == TermNe || subtype == TermEq)) ||
        (top_symbol == NonTermTermListTail && (subtype == TermMul || subtype == TermDiv)) ||
        (top_symbol == NonTermExprBoolOrRest && (subtype == TermOr)) ||
        (top_symbol == NonTermExprBoolAndTail && (subtype == TermAnd)))
    {
        while (!g_queue_is_empty(op_stack) && is_high_precedence(curr_tok, op_stack))
            create_binop(ast_stack, op_stack);

        g_queue_push_head(op_stack, node);
    }
}

void add_statement(ASTNode *node, ASTNode *stmt, gpointer block_stack, bool stmt_has_block)
{
    int blocks_length = g_queue_get_length(block_stack);

    if (stmt_has_block && blocks_length > 1)
    {
        ASTNode *block_head = g_queue_pop_head(block_stack);
        ASTNode *second_block_head = g_queue_pop_head(block_stack);

        add_child(second_block_head, stmt);
        g_queue_push_head(block_stack, second_block_head);
        g_queue_push_head(block_stack, block_head);
        return;
    }
    if (blocks_length > 1 || (!stmt_has_block && blocks_length == 1))
    {

        ASTNode *a = g_queue_pop_head(block_stack);
        add_child(a, stmt);
        g_queue_push_head(block_stack, a);
        return;
    }

    add_child(node, stmt);
}

int precedence(TokenSubtype t)
{
    if (t == TermPow)
        return 3;
    if (t == TermMul || t == TermDiv)
        return 2;
    if (t == TermPlus || t == TermMinus)
        return 1;
    if (t == TermLe || t == TermLt || t == TermGe || t == TermGt)
        return 4;
    if (t == TermEq || t == TermNe)
        return 3;
    if (t == TermAnd)
        return 2;
    if (t == TermOr)
        return 1;

    return 0;
}

bool is_high_precedence(Token curr_token, gpointer op_stack)
{
    ASTNode *st_node = g_queue_peek_head(op_stack);

    return precedence(st_node->token->subtype) >= precedence(curr_token.subtype);
}

void parser_non_recurs_only_syntax_check()
{
    ParseTable table = get_table();
    GQueue *stack = g_queue_new();

    g_queue_push_head(stack, GINT_TO_POINTER(NonTermProgram));

    while (!g_queue_is_empty(stack))
    {
        int top = GPOINTER_TO_INT(g_queue_pop_head(stack));
        Token curr_token = peek_current_token();

        if (top == TermEof && curr_token.type == TOKEN_EOF)
        {
            g_queue_free(stack);
            printf("ok!");
            return;
        }

        if (top >= NonTermProgram && top <= NonTermIdentifierArray)
        {
            int *prods = table[top][curr_token.subtype];

            if (prods != NULL)
            {
                int *p = prods;
                while (*p != -1)
                    p++;

                for (p--; p >= prods; p--)
                    if (*p != TermEpsilon)
                        g_queue_push_head(stack, GINT_TO_POINTER(*p));

                // perform_ast_node(top, curr_token, ast_stack, op_stack);
            }
            else
            {
                error_expected_non_term(top);
                g_queue_free(stack);
                return;
            }
        }
        else if (top == curr_token.subtype)
        {

            peek_next_token();
        }
        else

        {
            printf("Expected: %s, found: %s\n",
                   terminal_enum_to_str(top),
                   terminal_enum_to_str(curr_token.subtype));
            g_queue_free(stack);
            return;
        }
    }

    g_queue_free(stack);
    return;
}
