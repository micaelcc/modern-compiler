#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ast.h"

void test_create_node()
{
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = 1;
    token->value = strdup("token_example");

    ASTNode *node = create_node("node1", token);

    assert(node != NULL);
    assert(strcmp(node->value, "node1") == 0);
    assert(node->token != NULL);
    assert(node->token->type == 1);
    assert(strcmp(node->token->value, "token_example") == 0);
    assert(node->childrens == NULL);
    assert(node->child_count == 0);

    free(node->token);
    free(node->value);
    free(node);
}

void test_add_child()
{
    ASTNode *parent = create_node("parent", NULL);
    ASTNode *child1 = create_node("child1", NULL);
    ASTNode *child2 = create_node("child2", NULL);

    assert(parent->child_count == 0);

    add_child(parent, child1);
    assert(parent->child_count == 1);
    assert(parent->childrens[0] == child1);

    add_child(parent, child2);
    assert(parent->child_count == 2);
    assert(parent->childrens[1] == child2);

    free(parent->childrens);
    free(child1->value);
    free(child1);
    free(child2->value);
    free(child2);
    free(parent->value);
    free(parent);
}

void test_free_ast()
{
    ASTNode *parent = create_node("parent", NULL);
    ASTNode *child1 = create_node("child1", NULL);
    ASTNode *child2 = create_node("child2", NULL);
    add_child(parent, child1);
    add_child(parent, child2);

    free_ast(parent);
}

int main()
{
    test_create_node();
    test_add_child();
    test_free_ast();

    return 0;
}