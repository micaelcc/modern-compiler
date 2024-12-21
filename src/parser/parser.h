#include "../token/token.h"


typedef struct Node {
    Token *token;
    struct Node* left;
    struct Node* right;
} AstNode;