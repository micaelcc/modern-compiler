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

void advance_next_char(char *, size_t *, size_t *, size_t *);
Token *make_tokens(const char[]);
char *make_bool_operator(const char[], size_t *);
char *make_lexema(const char[], size_t *, LexemaCondition);
bool is_end_of_statement(char);
bool is_number(char);
bool is_identifier(char);
bool is_character_of_str(char);
bool is_string(char);
bool is_arith_operator(char);
bool is_bool_operator(char, char);
bool is_symbol(char);

#endif /* __LEX_H__ */