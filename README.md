# modern-compiler
Modern Compiler Implementation in C

## Grammar

```md
<expr>               ::= <term> ((MINUS | PLUS) <term>)* ; | <assign_expr>
<assign_expr>        ::= [DEF_VAR] ID ASSIGN <expr> 
<term>               ::= <factor> ((MUL | DIV) <factor>)*
<factor>             ::= (PLUS | MINUS)* <factor>
<factor>             ::= <pow> | <atom>
<pow>                ::= [LPAR] <expr> [RPAR] ** [LPAR] <expr> [RPAR] 
<atom>               ::= INT | FLOAT | STRING | ID | LPAR <expr> RPAR
<expr_bool>          ::= NOT <expr_bool> | <expr_bool> <op_bool> (<expr_bool>)* | <atom>
<op_bool>            ::= LT | GT | LE | GE | NE | EQ | AND | OR
<while_statement>    ::= WHILE LPAR <expr_bool> RPAR <compound_statement>
<if_statement>       ::= IF LPAR <expr_bool> RPAR <compound_statement> (<else_statement>)*
<else_statement>     ::= ELSE (<if_statement> | <compound_statement>)
<compound_statement> ::= LBRACE { <statement> }* RBRACE
<statement>          ::= <expr> | <if_statement> | <while_statement>
```

### Constants
| Const | Meaning | Value |
|-----------|--------------------|-------------|
| MINUS     | Minus operator                 | -             |
| PLUS      | Plus Operator                  | +             |
| DEF_VAR   | Assign keyword                 | let           |
| ID        | Identifier                     | var1, var2    |
| ASSIGN    | Assign operator                | =             |
| MUL       | Mul operator                   | *             |
| DIV       | Div operator                   | /             |
| LPAR      | Left paren                     | (             |
| RPAR      | Right paren                    | )             |
| NOT       | Not operator                   | !             |
| LT        | Less than operator             | <             |
| GT        | Greater than operator          | >             |
| LE        | Less than or equal operator    | <=            |
| GE        | Greater than or equal operator | >=            |
| NE        | Not equal operator             | !=            |
| AND       | And operator                   | and           |
| OR        | Or operator                    | or            |
| EQ        | Equal operator                 | ==            |
| WHILE     | While keyword                  | while         |
| IF        | If keyword                     | if            |
| ELSE      | Else keyword                   | else          |
| LBRACE    | Left brace                     | {             |
| RBRACE    | Right brace                    | }             |


