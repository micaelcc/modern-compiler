#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "constants.h"

void test_constants()
{
    assert(IF != NULL);
    assert(DEF_VAR != NULL);
    assert(WHILE != NULL);
    assert(FOR != NULL);
    assert(ELSE != NULL);
    assert(ELSEIF != NULL);
    assert(DIGITS != NULL);
    assert(LETTERS != NULL);
    assert(LETTERS_DIGITS != NULL);
    assert(UNDER != NULL);
    assert(INTEGER != NULL);
    assert(FLOAT != NULL);
    assert(STRING != NULL);
    assert(OP != NULL);
    assert(ID != NULL);
    assert(KEY != NULL);
    assert(E0F != NULL);
    assert(PLUS != NULL);
    assert(MINUS != NULL);
    assert(MUL != NULL);
    assert(DIV != NULL);
    assert(LPAR != NULL);
    assert(RPAR != NULL);
    assert(LSQUARE != NULL);
    assert(RSQUARE != NULL);
    assert(RBRACE != NULL);
    assert(LBRACE != NULL);
    assert(EQ != NULL);
    assert(ASSIGN != NULL);
    assert(POW != NULL);
    assert(QUOTES != NULL);
    assert(LT != NULL);
    assert(GT != NULL);
    assert(LE != NULL);
    assert(GE != NULL);
    assert(NE != NULL);
    assert(AND != NULL);
    assert(OR != NULL);
    assert(COMMA != NULL);
    assert(NOT != NULL);
}

int main()
{
    test_constants();

    return 0;
}