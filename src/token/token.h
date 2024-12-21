#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constants/constants.h"
typedef enum {
    TOKEN_PLUS = 1,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LT,
    TOKEN_INTEGER,
    TOKEN_DEF_VAR,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_KEY,
    TOKEN_GT,
    TOKEN_LE,
    TOKEN_GE,
    TOKEN_NE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_EQ,
    TOKEN_STRING,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_RBRACE,
    TOKEN_LBRACE,
    TOKEN_EOF,
    TOKEN_LSQUARE,
    TOKEN_RSQUARE,
    TOKEN_POW,
    TOKEN_QUOTES,
    TOKEN_COMMA,
    TOKEN_NOT,
    TOKEN_UNK = -1
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

Token create_token(TokenType, char *);
Token *push_token(Token, Token *, int *);
void print_tokens(Token *);

#endif /* __TOKEN_H__ */
