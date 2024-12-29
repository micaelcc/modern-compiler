#include "../token/token.h"
#include "./implementations/recursive.h"

typedef struct Node {
    Token *token;
    struct Node* left;
    struct Node* right;
} AstNode;

void execute_recursive_descendent();