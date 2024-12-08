#include "lex.h"

Token *make_tokens(const char value[])
{
    size_t current = 0;
    size_t column = -1;
    size_t row = -1;

    char c = value[current];
    int index_tokens = 0;
    Token *tokens = NULL;

    while (c != '\0')
    {
        char *lexema = NULL;
        char *type = NULL;

        if (strchr(" \t", c))
        {
            advance_next_char(value, &row, &column, &current);
        }
        else if (is_end_of_statement(c)) {
            lexema = malloc(2 * sizeof(char));
            sprintf(lexema, "%c", c);
            advance_next_char(value, &row, &column, &current);
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
        else if (is_arith_operator(c))
        {
            lexema = malloc(2 * sizeof(char));
            sprintf(lexema, "%c", c);
            advance_next_char(value, &row, &column, &current);
        }
        else if (is_bool_operator(c, value[current+1]))
        {
            lexema = make_bool_operator(value, &current);
            type = NULL;
        }
        else if (is_symbol(c))
        {
            lexema = malloc(2 * sizeof(char));
            sprintf(lexema, "%c", c);
            advance_next_char(value, &row, &column, &current);
        }
        else
        {
            advance_next_char(value, &row, &column, &current);
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

void advance_next_char(char *s, size_t *row, size_t *column, size_t *index) {
    (*index) += 1;
    (*column) += 1;

    if (s[*index] == '\n') {
        (*row) += 1;
        (*column) = 0;
    }
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

char *make_bool_operator(const char value[], size_t *index) {
    const size_t start = *index;
    size_t i;
    
    for (i = 0; BOOL_OPERATORS[i]; i++) {
        if (BOOL_OPERATORS[i][0] == value[*index] 
            && BOOL_OPERATORS[i][1] == value[(*index)++]) {
            (*index)+=1;
            break;
        }
    }
    
    return substring(value, start, (*index-1));
}

bool is_end_of_statement(char c) {
    return strchr(END_OF_STATEMENT, c) != NULL;
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

bool is_arith_operator(char c)
{
    size_t i;
    for (i = 0; ARITH_OPERATORS[i]; i++)
        if (strchr(ARITH_OPERATORS[i], c))
            return 1;

    return 0;
}

bool is_bool_operator(char c, char next)
{
    size_t i;

    if (c == ASSIGN[0] && next != ASSIGN[0])
        return 0;

    for (i = 0; BOOL_OPERATORS[i]; i++)
        if (BOOL_OPERATORS[i][0] == c)
            return 1;

    return 0;
}

bool is_symbol(char c)
{
    size_t i;
    for (i = 0; SYMBOLS[i]; i++)
        if (strchr(SYMBOLS[i], c))
            return 1;

    return 0;
}

