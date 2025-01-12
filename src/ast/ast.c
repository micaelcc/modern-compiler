#include "ast.h"

ASTNode *create_node(char *value, Token *t)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->value = strdup(value);
    node->token = t != NULL ? t : NULL;
    node->childrens = NULL;
    node->child_count = 0;
    return node;
}

void add_child(ASTNode *parent, ASTNode *child)
{
    parent->childrens = (ASTNode **)realloc(parent->childrens, sizeof(ASTNode *) * (parent->child_count + 1));
    parent->childrens[parent->child_count++] = child;
}

void free_ast(ASTNode *node)
{
    for (int i = 0; i < node->child_count; i++)
    {
        free_ast(node->childrens[i]);
    }
    free(node->childrens);
    free(node->token);
    free(node);
}
void print_ast(ASTNode *node, int depth, int is_last_child)
{
    for (int i = 0; i < depth; i++)
    {
        printf("   ");
    }

    printf("└── ");

    printf("%s\n", node->value);

    for (int i = 0; i < node->child_count; i++)
    {
        print_ast(node->childrens[i], depth + 1, i == node->child_count - 1);
    }
}