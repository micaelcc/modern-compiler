#include "lex.h"

Token *make_tokens(const char value[])
{
    size_t current = 0;
    char c = value[current];
    int index_tokens = 0;
    Token *tokens = NULL;

    while (c != '\0')
    {
        char *lexema = NULL;
        char *type = NULL;

        if (strchr(" \t", c))
        {
            current++;
        }
        else if (is_number(c))
        {
            lexema = make_lexema(value, &current, is_number);
            type = strchr(lexema, '.') ? FLOAT : INTEGER;
        }
        else if (is_identifier(c))
        {
            lexema = make_lexema(value, &current, is_identifier);
            type = contains(KEYWORDS, lexema) ? KEY : ID;
        }
        else if (is_string(c))
        {
            lexema = make_lexema(value, &current, is_character_of_str);
            type = STRING;
        }
        else if (is_operator(c))
        {
            lexema = malloc(2 * sizeof(char));
            sprintf(lexema, "%c", c);
            current++;
        }
        else if (is_paren_or_bracket(c))
        {
            lexema = malloc(2 * sizeof(char));
            sprintf(lexema, "%c", c);
            current++;
        }
        else
        {
            current++;
        }

        if (lexema)
        {
            Token t = create_token(type, lexema);

            tokens = push_token(t, tokens, &index_tokens);
        }

        c = value[current];
    }

    tokens = push_token(create_token(NULL, E0F), tokens, &index_tokens);

    return tokens;
}

char *make_lexema(const char value[], size_t *index, LexemaCondition cond)
{
    const size_t start = *index;

    if (value[start] == QUOTES[0])
        (*index)++;

    while (cond(value[*index]))
        (*index)++;

    if (value[start] == QUOTES[0])
        (*index)++;

    return substring(value, start, (*index - 1));
}

bool is_identifier(char c)
{
    return strchr(LETTERS_DIGITS, c) || c == '_';
}

bool is_number(char c)
{
    return strchr(DIGITS, c) || c == '.';
}

bool is_character_of_str(char c)
{
    return (isalnum(c) || c == ' ');
}

bool is_string(char c)
{
    return strchr(QUOTES, c) != NULL;
}

bool is_operator(char c)
{
    size_t i;
    for (i = 0; OPERATORS[i]; i++)
        if (strchr(OPERATORS[i], c))
            return 1;

    return 0;
}

bool is_paren_or_bracket(char c)
{
    return c == LPAR[0] || c == RPAR[0] || c == RSQUARE[0] || c == LSQUARE[0];
}

