#include "../token/token.h"

typedef struct Node
{
    char *value;
    Token *token;
    struct Node **childrens;
    size_t child_count;
} ASTNode;

ASTNode *create_node(char *value, Token *t);

void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node, int depth, int is_last_child);
void free_ast(ASTNode *node);