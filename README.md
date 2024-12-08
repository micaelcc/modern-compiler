# modern-compiler
Modern Compiler Implementation in C

## Grammar

```md
# Syntactic Grammar

<program>            ::= <statement>*
<statement>          ::= <expr> 
                       | <if_statement> 
                       | <while_statement>
<compound_statement> ::= "{" <statement>* "}"

<expr>               ::= <assign_expr> 
                       | <arith_expr> 
<arith_expr>         ::= <term> (("-" | "+") <term>)*
<assign_expr>        ::= ("let" <space>)? <identifier> "=" <expr> ";"
<term>               ::= <factor> (("*" | "/") <factor>)*
<factor>             ::= ("+" | "-")* (<pow> | <atom>)
<priority_factor>    ::= "(" <arith_expr> ")" | <pow> | <atom>
<pow>                ::= <priority_factor> "^" <priority_factor>               
<atom>               ::= <digit>+ 
                       | <identifier> 
                       | <string> 
                       | "(" <expr> ")"

<expr_bool>          ::= <expr_bool_or> 
                       | "(" <expr_bool_or> ")"
<expr_bool_or>       ::= <expr_bool_and> (<space> "||" <space> <expr_bool_and>)*
<expr_bool_and>      ::= <expr_bool_not> (<space> "&&" <space> <expr_bool_not>)*
<expr_bool_not>      ::= "!" <expr_bool_not> 
                       | <expr_bool_rel>
                       | "(" <expr_bool> ")"
<expr_bool_rel>      ::= <arith_expr> <op_bool> <arith_expr> 
                       | <atom>
<op_bool>            ::= "<" 
                       | ">" 
                       | "<=" 
                       | ">=" 
                       | "!=" 
                       | "==" 
                       | "&&" 
                       | "||"

<while_statement>    ::= "while" "(" <expr_bool> ")" <compound_statement>

<if_statement>       ::= "if" "(" <expr_bool> ")" <compound_statement> (<else_statement>)*

<else_statement>     ::= "else" (<if_statement> | <compound_statement>)

<identifier>         ::= <letter> <letter_or_digit>*
<string>             ::= <quote> (<letter_or_digit> | "_" | " ")+ <quote>
<letter>             ::= [a-z] 
                       | [A-Z] 
                       | "_"
<letter_or_digit>    ::= <letter> 
                       | <digit>
<digit>              ::= [0-9]
<space>              ::= " "
<new_line>           ::= "\n"
<quote>              ::= "\""
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


