#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constants/constants.h"
typedef struct
{
    char *type;
    char *value;
} Token;

Token create_token(char *, char *);
Token *push_token(Token, Token *, int *);
void print_tokens(Token *);

#endif /* __TOKEN_H__ */
