#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "token.h"

void test_create_token()
{
    Token token_result = create_token(TOKEN_KEY, "else");

    assert(token_result.type == TOKEN_KEY);
    assert(strcmp(token_result.value, "else") == 0);
}

void test_push_token()
{
    Token *l = (Token*)malloc(sizeof(Token) * 2);
    int size = 2;
    l[0].type = TOKEN_KEY;
    l[0].value = "let";
    l[0].subtype = TermDefVar;
    l[1].type = TOKEN_ID;
    l[1].value = "identifier";
    l[1].subtype = TermIdentifier;

    Token *new_l = push_token((Token){TOKEN_SEMICOLON, TermSemi, ";"}, l, &size);

    assert(size == 3);
    assert(new_l[size - 1].type == TOKEN_SEMICOLON);
}

int main()
{
    test_create_token();
    test_push_token();

    printf("Token tests passed.\n");
    
    return 0;
}