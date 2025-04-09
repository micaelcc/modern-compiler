#ifndef __AST_H__
#define __AST_H__
#include <glib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../token/token.h"

typedef struct Node
{
    char *value;
    Token *token;
    struct Node **childrens;
    size_t child_count;
} ASTNode;

ASTNode *create_node(char *value, Token t);

void add_child(ASTNode *, ASTNode *);
void print_ast(ASTNode *);
void free_ast(ASTNode *);
void update_node_for(ASTNode *, size_t, ASTNode *);
ASTNode *create_binop(gpointer, gpointer);
ASTNode *create_let(Token *, ASTNode *, gpointer);
ASTNode *create_reassign(Token *, ASTNode *, gpointer);
ASTNode *create_if_and_elseif(gpointer, gpointer, char *);
ASTNode *create_else(gpointer, gpointer);
ASTNode *create_for(gpointer, gpointer);

#endif // __AST_H__