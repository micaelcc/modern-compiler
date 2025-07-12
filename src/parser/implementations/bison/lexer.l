%{
#include "parser.tab.h"
#include <string.h>
%}

%%
"let"        { return __LET; }
"if"         { return __IF; }
"else"       { return __ELSE; }
"elseif"     { return __ELSEIF; }
"while"      { return __WHILE; }
"for"        { return __FOR; }
"=="         { return __EQ; }
"="          { return __ASSIGN; }
"!="         { return __NE; }
"<="         { return __LE; }
">="         { return __GE; }
"<"          { return __LT; }
">"          { return __GT; }
"&&"         { return __AND; }
"||"         { return __OR; }
"!"          { return __NOT; }
"+"          { return __PLUS; }
"-"          { return __MINUS; }
"*"          { return __STAR; }
"/"          { return __SLASH; }
"^"          { return __POW; }
";"          { return __SEMICOLON; }
","          { return __COMMA; }
"("          { return __LPAREN; }
")"          { return __RPAREN; }
"{"          { return __LBRACE; }
"}"          { return __RBRACE; }
"["          { return __LBRACK; }
"]"          { return __RBRACK; }

[0-9]+"."[0-9]+ { yylval.f = atof(yytext); return __FLOAT; }
[0-9]+          { yylval.i = atoi(yytext); return __INTEGER; }
\"[^\"]*\"      { yylval.str = strdup(yytext); return __STRING; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return __IDENTIFIER;
}

[ \t\r\n]+      { /* ignora espaços */ }
.               { printf("caractere inválido: %s\n", yytext); return -1; }
%%
