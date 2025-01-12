#include "constants.h"

const char IF[] = "if";
const char DEF_VAR[] = "let";
const char WHILE[] = "while";
const char FOR[] = "for";
const char ELSE[] = "else";
const char ELSEIF[] = "elseif";

const char DIGITS[] = "0123456789";
const char LETTERS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char LETTERS_DIGITS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char UNDER[] = "_";

const char INTEGER[] = "INTEGER";
const char FLOAT[] = "FLOAT";
const char STRING[] = "STRING";
const char OP[] = "OP";
const char ID[] = "ID";
const char KEY[] = "KEY";
const char E0F[] = "$EOF";

const char PLUS[] = "+";
const char MINUS[] = "-";
const char MUL[] = "*";
const char DIV[] = "/";
const char LPAR[] = "(";
const char RPAR[] = ")";
const char LSQUARE[] = "[";
const char RSQUARE[] = "]";
const char RBRACE[] = "}";
const char LBRACE[] = "{";
const char EQ[] = "==";
const char ASSIGN[] = "=";
const char POW[] = "^";
const char QUOTES[] = "\"";
const char LT[] = "<";
const char GT[] = ">";
const char LE[] = "<=";
const char GE[] = ">=";
const char NE[] = "!=";
const char AND[] = "&&";
const char OR[] = "||";
const char COMMA[] = ",";
const char NOT[] = "!";

const char *KEYWORDS[] = {
    DEF_VAR,
    IF,
    ELSE,
    WHILE,
    FOR,
    AND,
    ELSEIF,
    OR,
    NULL};

const char *ARITH_OPERATORS[] = {
    PLUS,
    MINUS,
    POW,
    DIV,
    MUL,
    NULL};

const char *SYMBOLS[] = {
    ASSIGN,
    LPAR,
    RPAR,
    LSQUARE,
    RSQUARE,
    RBRACE,
    LBRACE,
    COMMA,
    NOT,
    NULL};

const char *BOOL_OPERATORS[] = {
    LT,
    GT,
    LE,
    GE,
    NE,
    AND,
    OR,
    EQ,
    NULL};

const char END_OF_STATEMENT[] = ";";