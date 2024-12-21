#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *input_string();
char *substring(const char *, int, int);
bool contains(const char *[], const char[]);

#endif // STRINGUTILS_H