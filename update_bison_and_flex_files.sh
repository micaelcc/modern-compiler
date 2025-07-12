#!/bin/bash

echo "Updating src/parser/implementations/bison/parser.tab.c ..."
    bison -d -o src/parser/implementations/bison/parser.tab.c src/parser/implementations/bison/parser.y
echo "done."

echo "Updating src/parser/implementations/bison/lex.yy.c ..."
    flex --header-file=src/parser/implementations/bison/lex.yy.h -o src/parser/implementations/bison/lex.yy.c src/parser/implementations/bison/lexer.l
echo "done."

