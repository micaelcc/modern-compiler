#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constants/constants.h"
typedef enum
{
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

typedef enum
{
    TermIdentifier = 1000,
    TermDefVar,
    TermFor,
    TermIf,
    TermWhile,
    TermLbrace,
    TermRbrace,
    TermEof,
    TermAssign,
    TermSemi,
    TermMinus,
    TermPlus,
    TermFloat,
    TermString,
    TermInteger,
    TermLparen,
    TermRparen,
    TermLbracket,
    TermRbracket,
    TermOr,
    TermAnd,
    TermComma,
    TermLe,
    TermLt,
    TermGe,
    TermGt,
    TermNe,
    TermEq,
    TermMul,
    TermDiv,
    TermPow,
    TermNot,
    TermElseIf,
    TermEpsilon,
    TermElse
} TokenSubtype;

typedef struct
{
    TokenType type;
    int subtype;
    char *value;
} Token;

int get_token_subtype(TokenType, char *);
Token create_token(TokenType, char *);
Token *push_token(Token, Token *, int *);
void print_tokens(Token *);
Token get_undef_token();

#endif /* __TOKEN_H__ */
