#include "token.h"

Token create_token(char *type, char *value)
{
    Token new_token;

    new_token.type = type;
    new_token.value = value;

    return new_token;
}

Token *push_token(Token t, Token *l, int *size)
{
    Token *new_tokens = realloc(l, (*size + 1) * sizeof(Token));

    if (new_tokens == NULL)
    {
        printf("Unable to allocate memory.\n");
        exit(1);
    }

    l = new_tokens;

    l[*size].type = t.type;
    l[*size].value = t.value;

    (*size)++;

    return l;
}

void print_tokens(Token *t)
{
    printf("[");

    for (; strcmp(t->value, E0F) != 0; t++)
        if (t->type)
            printf("%s:%s, ", t->type, t->value);
        else
            printf("%s, ", t->value);

    printf("$EOF]\n");
}