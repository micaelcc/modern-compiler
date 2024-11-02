#include "constants.h"

const char IF[] = "if";
const char DEF_VAR[] = "let";
const char WHILE[] = "while";
const char FOR[] = "for";

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
const char EQ[] = "=";
const char POW[] = "^";
const char QUOTES[] = "\"";

const char *KEYWORDS[] = {
    DEF_VAR,
    IF,
    WHILE,
    FOR,
    NULL};

const char *OPERATORS[] = {
    PLUS,
    MINUS,
    EQ,
    POW,
    LPAR,
    RPAR,
    DIV,
    MUL,
    NULL};