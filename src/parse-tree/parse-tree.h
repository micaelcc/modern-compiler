#ifndef __PARSE_TREE_H__
#define __PARSE_TREE_H__
#define START_TREE_SIZE 256
#include <stdlib.h>
#include <stdbool.h>
#include "../token/token.h"
#include "../parser/table/table.h"
#include "time.h"
typedef struct Node
{
    int symbol;
    int *_integer; // INTEGER(1)
    float *_float; // FLOAT(2.2)
    char *_str;    // STRING ("id")
    struct Node *first_child;
    struct Node *last_child;
    struct Node *next_sibling;
} ParseTree;

ParseTree *create_parse_tree_node(int, Token);
void add_child_front(ParseTree *, ParseTree *);
void add_child_back(ParseTree *, ParseTree *);
void print_tree_rec(ParseTree *, int, int *, bool);
void print_tree(ParseTree *);
void print_tree_inline_rec(ParseTree *);
void print_tree_inline(ParseTree *);
void free_parse_tree(ParseTree *);
#endif // __PARSE_TREE_H__