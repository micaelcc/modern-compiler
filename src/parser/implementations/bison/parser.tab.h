/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_IMPLEMENTATIONS_BISON_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_IMPLEMENTATIONS_BISON_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    __INTEGER = 258,               /* __INTEGER  */
    __FLOAT = 259,                 /* __FLOAT  */
    __IDENTIFIER = 260,            /* __IDENTIFIER  */
    __STRING = 261,                /* __STRING  */
    __LET = 262,                   /* __LET  */
    __IF = 263,                    /* __IF  */
    __ELSE = 264,                  /* __ELSE  */
    __ELSEIF = 265,                /* __ELSEIF  */
    __WHILE = 266,                 /* __WHILE  */
    __FOR = 267,                   /* __FOR  */
    __PLUS = 268,                  /* __PLUS  */
    __MINUS = 269,                 /* __MINUS  */
    __STAR = 270,                  /* __STAR  */
    __SLASH = 271,                 /* __SLASH  */
    __POW = 272,                   /* __POW  */
    __LPAREN = 273,                /* __LPAREN  */
    __RPAREN = 274,                /* __RPAREN  */
    __LBRACE = 275,                /* __LBRACE  */
    __RBRACE = 276,                /* __RBRACE  */
    __LBRACK = 277,                /* __LBRACK  */
    __RBRACK = 278,                /* __RBRACK  */
    __EQ = 279,                    /* __EQ  */
    __ASSIGN = 280,                /* __ASSIGN  */
    __NE = 281,                    /* __NE  */
    __LT = 282,                    /* __LT  */
    __GT = 283,                    /* __GT  */
    __LE = 284,                    /* __LE  */
    __GE = 285,                    /* __GE  */
    __AND = 286,                   /* __AND  */
    __OR = 287,                    /* __OR  */
    __NOT = 288,                   /* __NOT  */
    __SEMICOLON = 289,             /* __SEMICOLON  */
    __COMMA = 290                  /* __COMMA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "src/parser/implementations/bison/parser.y"

    int i;
    float f;
    char* str;

#line 105 "src/parser/implementations/bison/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_IMPLEMENTATIONS_BISON_PARSER_TAB_H_INCLUDED  */
