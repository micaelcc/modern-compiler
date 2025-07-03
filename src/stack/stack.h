#ifndef __STACK_H__
#define __STACK_H__
#define STACK_INITIAL_SIZE 32000
#include <stdlib.h>
#include <stdbool.h>
#include "../parse-tree/parse-tree.h"

// This Stack implementation is not generic to maximize performance.
// It is specifically tailored for a fixed data type (e.g., int) to avoid overhead.
typedef struct
{
    int *data;
    int top;
    size_t size;
} StackInt;

typedef struct
{
    ParseTree **data;
    int top;
    size_t size;
} StackParseTree;

StackInt *st_create();
int st_pop(StackInt *st);
int st_empty(const StackInt *st);
void st_insert(StackInt *st, int value);

StackParseTree *st_pt_create();
ParseTree *st_pt_pop(StackParseTree *st);
int st_pt_empty(const StackParseTree *st);
void st_pt_insert(StackParseTree *st, ParseTree *node);

void st_free(StackInt *st, StackParseTree* st_pt);
#endif // __STACK_H__
