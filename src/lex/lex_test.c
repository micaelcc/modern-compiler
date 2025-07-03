#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lex.h"

void test_make_tokens()
{
    make_tokens("if(x>10){let result=\"Greater\";}else if(x==10){let result=\"Equal\";}else {let result=\"Lesser\";}");
    Token *l = get_tokens();

    TokenType assert_token_types[] = {
        (TokenType)12,
        (TokenType)2,
        (TokenType)11,
        (TokenType)13,
        (TokenType)5,
        (TokenType)3,
        (TokenType)24,
        (TokenType)12,
        (TokenType)11,
        (TokenType)21,
        (TokenType)20,
        (TokenType)22,
        (TokenType)23,
        (TokenType)12,
        (TokenType)12,
        (TokenType)2,
        (TokenType)11,
        (TokenType)19,
        (TokenType)5,
        (TokenType)3,
        (TokenType)24,
        (TokenType)12,
        (TokenType)11,
        (TokenType)21,
        (TokenType)20,
        (TokenType)22,
        (TokenType)23,
        (TokenType)12,
        (TokenType)24,
        (TokenType)12,
        (TokenType)11,
        (TokenType)21,
        (TokenType)20,
        (TokenType)22,
        (TokenType)23,
    };
    int i;
    for (i = 0; i < 35; i++)
    {
        assert(l[i].type == assert_token_types[i]);
    }

    assert(l[i].type == TOKEN_EOF);
}

void test_peek_current_token()
{
    make_tokens("if(x>10){let result=\"Greater\";}else if(x==10){let result=\"Equal\";}else {let result=\"Lesser\";}");

    peek_next_token(); // [if] --advance--> [(]

    Token current_token = peek_current_token();

    assert(current_token.type == TOKEN_LPAREN);
}

void test_peek_next_token()
{
    make_tokens("if(x>10){let result=\"Greater\";}else if(x==10){let result=\"Equal\";}else {let result=\"Lesser\";}");
    // current token: [if]

    Token next_token = peek_next_token();

    assert(next_token.type == TOKEN_LPAREN);
}

int main()
{
    test_make_tokens();
    test_peek_current_token();
    test_peek_next_token();

    printf("Lex tests passed.\n");
    return 0;
}