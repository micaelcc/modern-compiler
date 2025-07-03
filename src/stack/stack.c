#include "stack.h"

StackInt *st_create()
{
    StackInt *st = (StackInt*)malloc(sizeof(StackInt));
    if (!st)
        return NULL;
    st->data = (int*) malloc(STACK_INITIAL_SIZE * sizeof(int));
    if (!st->data)
    {
        free(st);
        return NULL;
    }
    st->top = -1;
    st->size = STACK_INITIAL_SIZE;
    return st;
}

int st_empty(const StackInt *st)
{
    return st->top == -1;
}

void st_insert(StackInt *st, int value)
{
    if ((size_t)(st->top + 1) == st->size)
    {
        size_t new_size = st->size * 2;
        int *new_data = (int*) realloc(st->data, new_size * sizeof(int));
        if (!new_data)
        {
            perror("Failed to realloc");
            exit(EXIT_FAILURE);
        }
        st->data = new_data;
        st->size = new_size;
    }
    st->data[++st->top] = value;
}

int st_pop(StackInt *st)
{
    return st->data[st->top--];
}

StackParseTree *st_pt_create() {
    StackParseTree *st = (StackParseTree*) malloc(sizeof(StackParseTree));
    if (!st) return NULL;
    st->data = (ParseTree**) malloc(STACK_INITIAL_SIZE * sizeof(ParseTree *));
    if (!st->data) {
        free(st);
        return NULL;
    }
    st->top = -1;
    st->size = STACK_INITIAL_SIZE;
    return st;
}

int st_pt_empty(const StackParseTree *st) {
    return st->top == -1;
}

void st_pt_insert(StackParseTree *st, ParseTree *node) {
    if ((size_t)(st->top + 1) == st->size) {
        size_t new_size = st->size * 2;
        ParseTree **new_data = (ParseTree**) realloc(st->data, new_size * sizeof(ParseTree *));
        if (!new_data) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        st->data = new_data;
        st->size = new_size;
    }
    st->data[++st->top] = node;
}

ParseTree* st_pt_pop(StackParseTree *st) {
    if (st->top < 0) {
        fprintf(stderr, "Stack underflow\n");
        return NULL;
    }
    return st->data[st->top--];
}

void st_free(StackInt *st, StackParseTree* st_pt)
{
    if (st) {
        free(st->data);
        free(st);
    }

    if (st_pt) {
        free(st_pt->data);
        free(st_pt);
    }
}
