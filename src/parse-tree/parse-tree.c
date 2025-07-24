#include "parse-tree.h"
#include "../stack/stack.h"
ParseTree *create_parse_tree_node(int symbol, Token tok)
{
    if (symbol == TermEof) {
        return NULL;
    }
    ParseTree *node = (ParseTree *)malloc(sizeof(ParseTree));
    if (!node)
    {
        fprintf(stderr, "Erro de alocação de ParseTree.");
        exit(1);
    }
    node->symbol = symbol;
    node->first_child = NULL;
    node->next_sibling = NULL;
    node->last_child = NULL;
    node->_str = NULL;
    node->_integer = NULL;
    node->_float = NULL;

    switch (symbol)
    {
    case TermIdentifier:
        node->_str = strdup(tok.value);
        break;
    case TermInteger:
        node->_integer = (int *)malloc(sizeof(int));
        *(node->_integer) = (int)atoi(tok.value);
        break;
    case TermFloat:
        node->_float = (float *)malloc(sizeof(float));
        *(node->_float) = (float)atof(tok.value);
        break;
    };

    return node;
}

void add_child_front(ParseTree *parent, ParseTree *child)
{
    if (!child)
        return;

    child->next_sibling = parent->first_child;
    parent->first_child = child;

    if (parent->last_child == NULL) {
        parent->last_child = child;
    }
}

void add_child_back(ParseTree *parent, ParseTree *child)
{
    if (!child)
        return;

    child->next_sibling = NULL;
    if (parent->first_child == NULL) {
        parent->first_child = child;
        parent->last_child = child;
    } else {
        parent->last_child->next_sibling = child;
        parent->last_child = child;
    }
}

void free_parse_tree(ParseTree *root) {
    if (!root) return;

    StackParseTree *stack = st_pt_create();
    st_pt_insert(stack, root);

    while (!st_pt_empty(stack)) {
        ParseTree *node = st_pt_pop(stack);
        for (ParseTree *child = node->first_child; child != NULL; ) {
            ParseTree *next = child->next_sibling;
            st_pt_insert(stack, child);
            child = next;
        }

        free(node->_integer);
        free(node->_float);
        free(node->_str);
        free(node);
    }

    st_free(NULL, stack);
}




void print_tree_node(ParseTree *node)
{
    const char *symbol_str = non_terminal_enum_to_str(node->symbol);
    if (!symbol_str)
        symbol_str = terminal_enum_to_str(node->symbol);

    if (node->_str != NULL)
        printf("%s(%s)", symbol_str ? symbol_str : "UNKNOWN", node->_str);
    else if (node->_float != NULL)
        printf("%s(%f)", symbol_str ? symbol_str : "UNKNOWN", *(node->_float));
    else if (node->_integer != NULL)
        printf("%s(%d)", symbol_str ? symbol_str : "UNKNOWN", *(node->_integer));
    else
        printf("%s", symbol_str ? symbol_str : "UNKNOWN");
}


void print_tree_inline_rec(ParseTree *node)
{
    if (!node)
        return;

    print_tree_node(node);

    if (node->first_child != NULL)
    {
        printf("(");
        ParseTree *child = node->first_child;
        while (child != NULL)
        {
            print_tree_inline_rec(child);

            if (child->next_sibling)
                printf(", ");
            child = child->next_sibling;
        }
    }
}

void print_tree_inline(ParseTree *root)
{
    print_tree_inline_rec(root);
    printf("\n");
}

void print_tree_rec(ParseTree *node, int level, int *levels, bool is_last)
{
    if (!node)
        return;

    printf("\t\t\t");

    for (int i = 0; i < level; i++) {
        if (levels[i])
            printf("│   ");
        else
            printf("    ");
    }

    if (level > 0) {
        if (is_last)
            printf("└── ");
        else
            printf("├── ");
    }

    print_tree_node(node);
    printf("\n");

    int count = 0;
    for (ParseTree *tmp = node->first_child; tmp; tmp = tmp->next_sibling)
        count++;

    ParseTree *child = node->first_child;
    int idx = 0;
    for (; child; child = child->next_sibling, idx++) {
        levels[level] = !is_last;
        print_tree_rec(child, level + 1, levels, idx == count - 1);
    }

    levels[level] = 0;
}


void print_tree(ParseTree *root)
{
    int levels[1000] = {0};
    print_tree_rec(root, 0, levels, true);
}
