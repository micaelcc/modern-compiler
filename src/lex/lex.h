#ifndef __LEX_H__
#define __LEX_H__
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include "../token/token.h"
#include "../utils/string-utils.h"
#include "../constants/constants.h"

typedef bool (*LexemaCondition)(char);

Token *make_tokens(const char[]);
char *make_lexema(const char[], size_t *, LexemaCondition);
bool is_number(char);
bool is_identifier(char);
bool is_character_of_str(char);
bool is_string(char);
bool is_operator(char);
bool is_paren_or_bracket(char);

#endif /* __LEX_H__ */