#include "../token/token.h"

typedef struct No {
    Token token;
    struct No* left;
    struct No* right;
} Node;

Node* create_number_node(Token* token) {
    Node* new_node = (Node*) malloc(sizeof(Node));

    if (new_node == NULL) {
        printf("Malloc error.");
        return NULL;
    }

    new_node->token.type = token->type;
    new_node->token.value = token->value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

Node * create_string_node(Token* token) {
    Node* new_node = (Node*) malloc(sizeof(Node));

    if (new_node == NULL) {
        printf("Malloc error.");
        return NULL;
    }

    new_node->token.type = token->type;
    new_node->token.value = token->value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

Node * create_binop_node(Token* token, Token* left, Token* right) {
    
}
Node* insert_number_in_ast(Node* l, Token token) {
    if (l == NULL) {
        return create_node(&token);
    }


}