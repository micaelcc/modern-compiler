#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdbool.h>

typedef struct {
    bool ONLY_SYNTAX_CHECK;
    bool PRINT_TOKENS;
    bool PRINT_PARSE_TREE;
    bool EXECUTE_RECURSIVE_DESCENT;
    bool EXECUTE_TABLE_DRIVEN;
    bool PRINT_PARSER_RESULT;
    bool EXECUTE_BISON;
} CompilerOptions;

#endif // __CONFIG_H__