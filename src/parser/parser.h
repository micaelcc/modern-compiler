#ifndef __PARSER_H__
#define __PARSER_H__
#define BMARK_REPS 10
#include <sys/resource.h>
#include <unistd.h>
#include "time.h"
#include "malloc.h"
#include "./implementations/recursive.h"
#include "./implementations/non-recursive.h"
#include "../token/token.h"
#include "../parse-tree/parse-tree.h"
#include "../config.h"
typedef void (*OnlySyntaxParser)(void);
typedef ParseTree *(*ParserWithTree)(void);
typedef struct
{
    char *parser;
    long double time_in_ms;
    long double memory_consumption_in_KB;
    long double memory_heap_consumption_in_KB;
    size_t tokens_analyzed;
    ParseTree *tree;
} ParserBMark;

ParserBMark execute_parser_bmark(CompilerOptions);
ParseTree *parse(CompilerOptions);

#endif // __PARSER_H__
