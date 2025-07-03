# modern-compiler
Modern Compiler Implementation in C
Project dedicated to testing two parsing methodologies for LL(1) grammars, as part of my Computer Science undergraduate thesis (Final Year Project).

#### Execute tests (Recursive Descent / Table-Driven -with and without Parse Tree)
Note: When running tests for the table-driven (non-recursive) parser, make sure the st_free calls in both stack and parse_tree_stack are commented out.
This is intentional — memory usage will be measured during benchmarking, and automatic deallocation would invalidate the results.
```bash
bash run_tests.sh
```

#### Generate random code
```bash
bash generate_code.sh --max-tokens 10000
```

#### Run 

```bash
make && ./bin/main
```

#### Tests 

```bash
make && make test
```

## Grammar

```md
(* Syntactic Grammar for EASYC *)

Program             ::= StatementList
CompoundStatement   ::= '{' StatementList '}'
StatementList       ::= Statement StatementListTail
StatementListTail   ::= Statement StatementListTail
                      | ε

Statement           ::= Expr ';'
                      | IfStatement
                      | WhileStatement
                      | ForStatement

Expr                ::= DeclExpr
                      | Identifier ReassignExprTail

ReassignExpr        ::= Identifier '=' ArithExpr
DeclExpr            ::= 'let' Identifier DeclExprAssign
DeclExprAssign      ::= '=' ArithExpr
                      | ε

ReassignExprTail    ::= '=' ArithExpr
                      | ArithExprTail

ArithExpr           ::= Term ArithExprTail
ArithExprTail       ::= ArithExprTailRest
ArithExprTailRest   ::= '+' Term ArithExprTailRest
                      | '-' Term ArithExprTailRest
                      | ε

Term                ::= Factor TermList
TermList            ::= TermListTail
TermListTail        ::= '*' Factor TermListTail
                      | '/' Factor TermListTail
                      | ε

Factor              ::= FactorRest
FactorRest          ::= '+' Pow
                      | '-' Pow
                      | Pow

Pow                 ::= Atom PowRest
PowRest             ::= '^' Pow
                      | ε

Atom                ::= 'INTEGER'
                      | 'FLOAT'
                      | Identifier
                      | 'STRING'
                      | '(' ArithExpr ')'
                      | Array

Array               ::= '[' ItemsArray ']'
ItemsArray          ::= Atom ItemsArrayTail
                      | ε

ItemsArrayTail      ::= ',' Atom ItemsArrayTail
                      | ε

ExprBool            ::= ExprBoolOr
ExprBoolOr          ::= ExprBoolAnd ExprBoolOrRest
ExprBoolOrRest      ::= '||' ExprBoolAnd ExprBoolOrRest
                      | ε

ExprBoolAnd         ::= ExprBoolNot ExprBoolAndTail
ExprBoolAndTail     ::= '&&' ExprBoolNot ExprBoolAndTail
                      |  ε

ExprBoolNot         ::= '!' ExprBoolNot 
                      | ExprBoolRel 

ExprBoolRel         ::= ArithExpr ExprBoolRelTail
ExprBoolRelTail     ::= OpBool ArithExpr 
                      | ε

ExprBoolRelFactor   ::= ArithExpr
OpBool              ::= '<'
                      | '>'
                      | '<='
                      | '>='
                      | '!='
                      | '=='

WhileStatement      ::= 'while' StatementStructure
IfStatement         ::= 'if' StatementStructure ElseifStatementList ElseStatement
ElseifStatementList ::= ElseifStatementTail
ElseifStatement     ::= 'elseif' StatementStructure
ElseifStatementTail ::= ElseifStatement ElseifStatementTail
                      | ε

ElseStatement       ::= 'else' CompoundStatement
                      | ε

StatementStructure  ::= '(' ExprBool ')' CompoundStatement
ForStatement        ::= 'for' '(' AssignExprList ';' ForExprBool ';' AssignExprList ')' CompoundStatement
ForExprBool         ::= ExprBool
                      | ε

AssignExprList      ::= ReassignExpr AssignExprTail
                      | ε

AssignExprTail      ::= ',' ReassignExpr AssignExprTail
                      | ε

Identifier          ::= 'IDENTIFIER' IdentifierArray
IdentifierArray     ::= '[' ArithExpr ']'
                      | ε
```