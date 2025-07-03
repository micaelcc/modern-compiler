#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

void test_st_create_and_free()
{
    Stack *st = st_create(4, false);
    assert(st != NULL);
    assert(st->size == 4);
    assert(st->top == -1);
    assert(st->use_parse_tree == false);
    assert(st->parse_tree_data == NULL);
    st_free(st);

    st = st_create(4, true);
    assert(st != NULL);
    assert(st->use_parse_tree == true);
    assert(st->parse_tree_data != NULL);
    st_free(st);
}

void test_st_insert_and_pop()
{
    Stack *st = st_create(2, false);
    st_insert(st, 10, NULL);
    st_insert(st, 20, NULL);

    StackItem item = st_pop(st);
    assert(item.symbol == 20);
    assert(item.parse_tree_node == NULL);

    item = st_pop(st);
    assert(item.symbol == 10);

    assert(st_empty(st));
    st_free(st);
}

void test_st_insert_and_pop_with_parse_tree()
{
    Stack *st = st_create(2, true);
    ParseTree *pt1 = (ParseTree *)malloc(sizeof(ParseTree));
    pt1->symbol = 100;
    ParseTree *pt2 = (ParseTree *)malloc(sizeof(ParseTree));
    pt2->symbol = 200;

    st_insert(st, 10, pt1);
    st_insert(st, 20, pt2);

    StackItem item = st_pop(st);
    assert(item.symbol == 20);
    assert(item.parse_tree_node == pt2);

    item = st_pop(st);
    assert(item.symbol == 10);
    assert(item.parse_tree_node == pt1);

    assert(st_empty(st));
    st_free(st);
    free(pt1);
    free(pt2);
}

void test_st_resize()
{
    Stack *st = st_create(1, true);
    ParseTree *pt = (ParseTree *)malloc(sizeof(ParseTree));
    pt->symbol = 123;

    st_insert(st, 1, pt);
    st_insert(st, 2, pt);

    assert(st->size >= 2);
    assert(st->top == 1);

    st_free(st);
    free(pt);
}

int main()
{
    test_st_create_and_free();
    test_st_insert_and_pop();
    test_st_insert_and_pop_with_parse_tree();
    test_st_resize();

    printf("Stack tests passed.\n");
    return 0;
}
