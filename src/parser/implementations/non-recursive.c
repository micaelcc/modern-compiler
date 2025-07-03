#include "non-recursive.h"


ParseTree *table_driven_parser()
{
    ParseTable table = get_table();
    ParseTree *root = create_parse_tree_node(NonTermProgram, (Token){TOKEN_UNK, -1, NULL});
    StackInt *stack = st_create();
    StackParseTree* parse_tree_stack = st_pt_create();
    st_insert(stack, NonTermProgram);
    st_pt_insert(parse_tree_stack, root);

    while (!st_empty(stack))
    {
        int top_symbol = st_pop(stack);
		ParseTree* top_parse_tree = st_pt_pop(parse_tree_stack);

        Token curr_token = peek_current_token();
        if (top_symbol == TermEof && curr_token.type == TOKEN_EOF)
        {
            st_free(stack, parse_tree_stack);
            return root;
        }

        if (top_symbol >= NonTermProgram && top_symbol <= NonTermIdentifierArray)
        {
            int *prods = table[top_symbol][curr_token.subtype];

            if (prods != NULL)
            {
                int *p = prods;

                while (*p != -1)
                    p++;

                for (p--; p >= prods; p--)
                    if (*p != TermEpsilon)
                    {
                        ParseTree *child = create_parse_tree_node(*p, curr_token);
                        add_child_front(top_parse_tree, child);
                        st_insert(stack, *p);
                        st_pt_insert(parse_tree_stack, child);
                    }
            }
            else
            {
                err_expected_term(table, top_symbol);
                st_free(stack, parse_tree_stack);
                return NULL;
            }
        }
        else if (top_symbol == curr_token.subtype)
        {
            peek_next_token();
        }
        else
        {
            err_expected(
                terminal_enum_to_str(top_symbol),
                terminal_enum_to_str(curr_token.subtype));
            st_free(stack, parse_tree_stack);
            return NULL;
        }
    }
    return NULL;
}

/* ######## FOR TESTING PURPOSES ########
 *
 * Parser without parse tree generation
 */
void table_driven_parser_only_syntax_check()
{
    ParseTable table = get_table();
    StackInt *stack = st_create();
    st_insert(stack, NonTermProgram);

    while (!st_empty(stack))
    {
        int top_symbol = st_pop(stack);

        Token curr_token = peek_current_token();
        if (top_symbol == TermEof && curr_token.type == TOKEN_EOF)
        {
            //st_free(stack, NULL);
            return;
        }

        if (top_symbol >= NonTermProgram && top_symbol <= NonTermIdentifierArray)
        {
            int *prods = table[top_symbol][curr_token.subtype];

            if (prods != NULL)
            {
                int *p = prods;

                while (*p != -1)
                    p++;

                for (p--; p >= prods; p--)
                    if (*p != TermEpsilon)
                        st_insert(stack, *p);
            }
            else
            {
                err_expected_term(table, top_symbol);
                //st_free(stack, NULL);
                return;
            }
        }
        else if (top_symbol == curr_token.subtype)
        {
            peek_next_token();
        }
        else
        {
            err_expected(
                terminal_enum_to_str(top_symbol),
                terminal_enum_to_str(curr_token.subtype));
            //st_free(stack, NULL);
            return;
        }
    }

    return;
}
