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

void set_tokens(Token *);
Token *get_tokens();
Token peek_current_token();
Token peek_next_token();
Token peek_next_token_no_advance();
size_t get_current_token_index();
void make_tokens(const char[]);
void advance_next_char(char *, size_t *, size_t *, size_t *);
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
size_t get_number_of_tokens();

#endif /* __LEX_H__ */