#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "parse-tree.h"

void test_create_parse_tree_node()
{
    Token tok;
    tok.type = TOKEN_ID;
    tok.value = strdup("id");

    ParseTree *node = create_parse_tree_node(99, tok);

    assert(node != NULL);
    assert(node->symbol == 99);

    if (node->_str) {
        assert(strcmp(node->_str, "id") == 0);
    }

    assert(node->child_count == 0);
    assert(node->childrens == NULL);

    free(node->_str);
    free(node);
    free(tok.value);
}

void test_add_child_parse_tree()
{
    Token dummy = { .type = TOKEN_AND, .value = NULL };

    ParseTree *parent = create_parse_tree_node(1, dummy);
    ParseTree *child1 = create_parse_tree_node(2, dummy);
    ParseTree *child2 = create_parse_tree_node(3, dummy);

    assert(parent->child_count == 0);

    add_child(parent, child1);
    assert(parent->child_count == 1);
    assert(parent->childrens[0] == child1);

    add_child(parent, child2);
    assert(parent->child_count == 2);
    assert(parent->childrens[1] == child2);

    // Free
    free(parent->childrens);
    free(child1);
    free(child2);
    free(parent);
}

void test_print_tree()
{
    Token dummy = { .type = TOKEN_COMMA, .value = NULL };

    ParseTree *root = create_parse_tree_node(1, dummy);
    ParseTree *child1 = create_parse_tree_node(2, dummy);
    ParseTree *child2 = create_parse_tree_node(3, dummy);

    add_child(root, child1);
    add_child(root, child2);

    print_tree(root);

    free(root->childrens);
    free(child1);
    free(child2);
    free(root);
}

int main()
{
    test_create_parse_tree_node();
    test_add_child_parse_tree();
    test_print_tree();

    printf("ParseTree tests passed.\n");
    return 0;
}
