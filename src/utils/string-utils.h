#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#define MAX_SPLIT_LEN 256
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

char *read_file_to_string(const char *);
void replace_char(char *, char, char);
int terminal_str_to_enum(const char *);
int non_terminal_str_to_enum(const char *);
void remove_char_by_index(char *, int);
void remove_char(char *, char);
void split_string(const char *, const char *, char[MAX_SPLIT_LEN][MAX_SPLIT_LEN], int *);
char *input_string();
char *substring(const char *, int, int);
bool contains(const char *[], const char[]);

#endif // STRINGUTILS_H