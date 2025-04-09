#include "ast.h"

ASTNode *create_node(char *value, Token t)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->token = NULL;

    if (t.subtype != -1)
    {
        node->token = (Token *)malloc(sizeof(Token));
        node->token->subtype = t.subtype;
        node->token->type = t.type;
        node->token->value = strdup(t.value);
    }

    node->value = strdup(value);
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
void print_ast_rec(ASTNode *node, int level, int *levels, bool is_last)
{
    if (node == NULL)
        return;

    for (int i = 0; i < level; i++)
    {
        if (levels[i])
        {
            printf("│   ");
        }
        else
        {
            printf("    ");
        }
    }

    if (level > 0)
    {
        if (is_last)
        {
            printf("└── ");
        }
        else
        {
            printf("├── ");
        }
    }

    printf("%s\n", node->value);

    levels[level] = (node->child_count > 0 && !is_last);

    for (size_t i = 0; i < node->child_count; i++)
    {
        print_ast_rec(node->childrens[i], level + 1, levels, i == node->child_count - 1);
    }

    levels[level] = 0;
}

void print_ast(ASTNode *root)
{
    int levels[100] = {0};
    print_ast_rec(root, 4, levels, 1);
}

ASTNode *create_binop(gpointer ast_stack, gpointer op_stack)
{
    ASTNode *op = g_queue_pop_head(op_stack);
    if (op->token->subtype == TermLparen)
        return NULL;

    Token binop = (Token){
        op->token->subtype,
        op->token->type,
        strdup(op->token->value)};

    ASTNode *binop_node = create_node(binop.value, binop);
    ASTNode *rigth = g_queue_pop_head(ast_stack);
    ASTNode *left = g_queue_pop_head(ast_stack);

    add_child(binop_node, left);
    add_child(binop_node, rigth);

    g_queue_push_head(ast_stack, binop_node);
    return binop_node;
}

ASTNode *create_let(Token *current_identifier, ASTNode *current_array, gpointer ast_stack)
{
    ASTNode *expr = g_queue_pop_head(ast_stack);
    Token let_tok = (Token){TermDefVar, TOKEN_DEF_VAR, DEF_VAR};
    Token id_tok = (Token){
        current_identifier->subtype,
        current_identifier->type,
        strdup(current_identifier->value)};
    ASTNode *id = create_node(strdup(current_identifier->value), id_tok);
    ASTNode *let = create_node(let_tok.value, let_tok);
    add_child(let, id);

    if (current_array)
        add_child(let, current_array);
    else if (strcmp(expr->token->value, id->value) != 0)
        add_child(let, expr);

    current_identifier->value = NULL;
    return let;
}

ASTNode *create_reassign(Token *current_identifier, ASTNode *current_array, gpointer ast_stack)
{
    ASTNode *node = create_node(ASSIGN, get_undef_token());
    Token id_tok = (Token){
        current_identifier->subtype,
        current_identifier->type,
        strdup(current_identifier->value)};

    add_child(node, create_node(id_tok.value, id_tok));
    if (current_array)
        add_child(node, current_array);
    else
        add_child(node, g_queue_peek_head(ast_stack));

    current_identifier->value = NULL;
    return node;
}

ASTNode *create_if_and_elseif(gpointer ast_stack, gpointer block_stack, char *stmt_value)
{
    ASTNode *pop = g_queue_pop_head(ast_stack);

    ASTNode *node = create_node(stmt_value, get_undef_token());
    Token block = {TermLbrace, TOKEN_LBRACE, LBRACE};
    ASTNode *block_node = create_node("block", block);
    add_child(node, pop);
    add_child(node, block_node);

    g_queue_push_head(block_stack, block_node);
    return node;
}

ASTNode *create_else(gpointer ast_stack, gpointer block_stack)
{
    ASTNode *node = create_node(ELSE, get_undef_token());
    Token block = {TermLbrace, TOKEN_LBRACE, LBRACE};
    ASTNode *block_node = create_node("block", block);
    add_child(node, block_node);

    g_queue_push_head(block_stack, block_node);
    return node;
}

ASTNode *create_for(gpointer ast_stack, gpointer block_stack)
{
    ASTNode *node = create_node(FOR, get_undef_token());
    Token block = {TermLbrace, TOKEN_LBRACE, LBRACE};
    ASTNode *block_node = create_node("block", block);
    add_child(node, create_node("assigns", get_undef_token()));
    add_child(node, create_node("expr", get_undef_token()));
    add_child(node, create_node("assigns", get_undef_token()));
    add_child(node, block_node);

    g_queue_push_head(block_stack, block_node);
    return node;
}

void update_node_for(ASTNode *root, size_t child_index, ASTNode *node)
{
    ASTNode *_for = root->childrens[root->child_count - 1];
    add_child(_for->childrens[child_index], node);
    return;
}