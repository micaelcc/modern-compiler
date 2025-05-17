#include "token.h"

Token create_token(TokenType t, const char *value)
{
    Token new_token;

    if (t == TOKEN_UNK)
    {
        t = get_token_type(value);
    }

    new_token.type = t;
    new_token.value = strdup(value);
    new_token.subtype = get_token_subtype(t, value);

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
    l[*size].subtype = t.subtype;

    (*size)++;

    return l;
}

TokenType get_token_type(const char *value)
{
    if (0 == strcmp(value, PLUS))
        return TOKEN_PLUS;

    if (0 == strcmp(value, MINUS))
        return TOKEN_MINUS;

    if (0 == strcmp(value, MUL))
        return TOKEN_MUL;

    if (0 == strcmp(value, DIV))
        return TOKEN_DIV;

    if (0 == strcmp(value, LT))
        return TOKEN_LT;

    if (0 == strcmp(value, GT))
        return TOKEN_GT;

    if (0 == strcmp(value, LE))
        return TOKEN_LE;

    if (0 == strcmp(value, GE))
        return TOKEN_GE;

    if (0 == strcmp(value, NE))
        return TOKEN_NE;

    if (0 == strcmp(value, AND))
        return TOKEN_AND;

    if (0 == strcmp(value, OR))
        return TOKEN_OR;

    if (0 == strcmp(value, EQ))
        return TOKEN_EQ;

    if (0 == strcmp(value, ASSIGN))
        return TOKEN_ASSIGN;

    if (0 == strcmp(value, END_OF_STATEMENT))
        return TOKEN_SEMICOLON;

    if (0 == strcmp(value, LPAR))
        return TOKEN_LPAREN;

    if (0 == strcmp(value, RPAR))
        return TOKEN_RPAREN;

    if (0 == strcmp(value, RBRACE))
        return TOKEN_RBRACE;

    if (0 == strcmp(value, LBRACE))
        return TOKEN_LBRACE;

    if (0 == strcmp(value, E0F))
        return TOKEN_EOF;

    if (0 == strcmp(value, LSQUARE))
        return TOKEN_LSQUARE;

    if (0 == strcmp(value, RSQUARE))
        return TOKEN_RSQUARE;

    if (0 == strcmp(value, RSQUARE))
        return TOKEN_RSQUARE;

    if (0 == strcmp(value, POW))
        return TOKEN_POW;

    if (0 == strcmp(value, COMMA))
        return TOKEN_COMMA;

    if (0 == strcmp(value, NOT))
        return TOKEN_NOT;

    return TOKEN_UNK;
}

const char *get_constant_by_type(TokenType type)
{
    switch (type)
    {
    case TOKEN_KEY:
        return KEY;
    case TOKEN_INTEGER:
        return INTEGER;
    case TOKEN_FLOAT:
        return FLOAT;
    case TOKEN_ID:
        return ID;
    default:
        return NULL;
    }
}

void print_tokens(Token *t)
{
    char *type;

    for (; strcmp(t->value, E0F) != 0; t++)
    {
        type = (char *)get_constant_by_type(t->type);

        if (type != NULL)
            printf("\033[32m ⦃%s:%s:%d⦄\033[0m", get_constant_by_type(t->type), t->value, t->subtype);
        else
            printf("\033[34m ⦃%d: %s:%d⦄\033[0m", t->type, t->value, t->subtype);
    }

    printf("\033[30m [%d:$EOF: %d]\033[0m\n\n", t->type, t->subtype);
}

int get_token_subtype(TokenType tt, const char *value)
{
    switch (tt)
    {
    case (TOKEN_PLUS):
        return TermPlus;
    case (TOKEN_LPAREN):
        return TermLparen;
    case (TOKEN_RPAREN):
        return TermRparen;
    case (TOKEN_LT):
        return TermLt;
    case (TOKEN_INTEGER):
        return TermInteger;
    case (TOKEN_DEF_VAR):
        return TermDefVar;
    case (TOKEN_MINUS):
        return TermMinus;
    case (TOKEN_MUL):
        return TermMul;
    case (TOKEN_DIV):
        return TermDiv;
    case (TOKEN_FLOAT):
        return TermFloat;
    case (TOKEN_ID):
        return TermIdentifier;
    case (TOKEN_GT):
        return TermGt;
    case (TOKEN_LE):
        return TermLe;
    case (TOKEN_GE):
        return TermGe;
    case (TOKEN_NE):
        return TermNe;
    case (TOKEN_AND):
        return TermAnd;
    case (TOKEN_OR):
        return TermOr;
    case (TOKEN_EQ):
        return TermEq;
    case (TOKEN_STRING):
        return TermString;
    case (TOKEN_ASSIGN):
        return TermAssign;
    case (TOKEN_SEMICOLON):
        return TermSemi;
    case (TOKEN_RBRACE):
        return TermRbrace;
    case (TOKEN_LBRACE):
        return TermLbrace;
    case (TOKEN_EOF):
        return TermEof;
    case (TOKEN_LSQUARE):
        return TermLbracket;
    case (TOKEN_RSQUARE):
        return TermRbracket;
    case (TOKEN_POW):
        return TermPow;
    case (TOKEN_QUOTES):
        return -1;
    case (TOKEN_COMMA):
        return TermComma;
    case (TOKEN_NOT):
        return TermNot;
    case (TOKEN_KEY):
        if (strcmp(value, DEF_VAR) == 0)
            return TermDefVar;
        else if (strcmp(value, FOR) == 0)
            return TermFor;
        else if (strcmp(value, WHILE) == 0)
            return TermWhile;
        else if (strcmp(value, IF) == 0)
            return TermIf;
        else if (strcmp(value, ELSEIF) == 0)
            return TermElseIf;
        else if (strcmp(value, ELSE) == 0)
            return TermElse;
        return -1;
    default:
        return -1;
    }
}

Token get_undef_token()
{
    Token tok;

    tok.subtype = -1;
    tok.type = TOKEN_UNK;
    tok.value = NULL;

    return tok;
}