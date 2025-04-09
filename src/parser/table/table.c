#include "table.h"

cJSON *table_json;
const int rowsSize = NonTermIdentifierArray + 1;
const int columnsSize = TermElse + 1;
const int columnsFirst = 1000;
ParseTable table;

void *load_file()
{
    FILE *file = fopen("./src/parser/table/grammar-expression-table.json", "r");
    if (!file)
    {
        perror("Open file error");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *str = (char *)malloc(size + 1);
    if (!str)
    {
        perror("Memory allocation failed");
        fclose(file);
        exit(1);
    }

    fread(str, 1, size, file);
    str[size] = '\0';

    fclose(file);

    table_json = cJSON_Parse(str);
}

int *search_table_json(enum NonTerminals non_terminal, int terminal)
{
    int num_prods, i;

    char *non_terminal_str = non_terminal_enum_to_str(non_terminal);
    char *terminal_str = terminal_enum_to_str(terminal);

    cJSON *non_terminal_row = cJSON_GetObjectItemCaseSensitive(table_json, non_terminal_str);

    char *array_prods = cJSON_Print(cJSON_GetObjectItemCaseSensitive(non_terminal_row, terminal_str));

    if (array_prods == NULL)
    {
        return NULL;
    }

    remove_char(array_prods, '"');

    char str_prods[256][256];
    num_prods = 0;

    remove_char_by_index(array_prods, 0);
    remove_char_by_index(array_prods, strlen(array_prods) - 1);

    replace_char(array_prods, ',', ':');
    split_string(array_prods, ":", str_prods, &num_prods);

    int *prods = malloc(sizeof(int) * num_prods);

    for (i = 0; i < num_prods; i++)
    {
        remove_char(str_prods[i], ' ');

        int index = non_terminal_str_to_enum(str_prods[i]);

        if (index == -1)
            index = terminal_str_to_enum(str_prods[i]);

        prods[i] = index;
    }

    prods[num_prods] = -1;
    return prods;
}

ParseTable get_table()
{
    return table;
}

void set_table()
{
    int i, j;
    table = (int ***)malloc(rowsSize * sizeof(int **));
    for (i = 0; i < rowsSize; i++)
    {
        table[i] = (int **)malloc(columnsSize * sizeof(int *));
        for (int j = columnsFirst; j < columnsSize; j++)
        {
            table[i][j] = search_table_json(i, j);
        }
    }
}

char *terminal_enum_to_str(int index)
{
    switch (index)
    {
    case (TermIdentifier):
        return "IDENTIFIER";
    case (TermDefVar):
        return "let";
    case (TermFor):
        return "for";
    case (TermIf):
        return "if";
    case (TermWhile):
        return "while";
    case (TermLbrace):
        return "{";
    case (TermRbrace):
        return "}";
    case (TermEof):
        return "$";
    case (TermAssign):
        return "=";
    case (TermSemi):
        return ";";
    case (TermMinus):
        return "-";
    case (TermPlus):
        return "+";
    case (TermFloat):
        return "FLOAT";
    case (TermString):
        return "STRING";
    case (TermInteger):
        return "INTEGER";
    case (TermLparen):
        return "(";
    case (TermRparen):
        return ")";
    case (TermLbracket):
        return "[";
    case (TermRbracket):
        return "]";
    case (TermOr):
        return "||";
    case (TermAnd):
        return "&&";
    case (TermComma):
        return ",";
    case (TermLe):
        return "<=";
    case (TermLt):
        return "<";
    case (TermGe):
        return ">=";
    case (TermGt):
        return ">";
    case (TermNe):
        return "!=";
    case (TermEq):
        return "==";
    case (TermMul):
        return "*";
    case (TermDiv):
        return "/";
    case (TermPow):
        return "^";
    case (TermNot):
        return "!";
    case (TermElseIf):
        return "elseif";
    case (TermElse):
        return "else";
    case (TermEpsilon):
        return "ε";
    case (NonTermProgram):
        return "Program";
    case (NonTermCompoundStatement):
        return "CompoundStatement";
    case (NonTermStatementList):
        return "StatementList";
    case (NonTermStatementListTail):
        return "StatementListTail";
    case (NonTermStatement):
        return "Statement";
    case (NonTermExpr):
        return "Expr";
    case (NonTermReassignExpr):
        return "ReassignExpr";
    case (NonTermDeclExpr):
        return "DeclExpr";
    case (NonTermDeclExprAssign):
        return "DeclExprAssign";
    case (NonTermReassignExprTail):
        return "ReassignExprTail";
    case (NonTermArithExpr):
        return "ArithExpr";
    case (NonTermArithExprTail):
        return "ArithExprTail";
    case (NonTermArithExprTailRest):
        return "ArithExprTailRest";
    case (NonTermTerm):
        return "Term";
    case (NonTermTermList):
        return "TermList";
    case (NonTermTermListTail):
        return "TermListTail";
    case (NonTermFactor):
        return "Factor";
    case (NonTermFactorRest):
        return "FactorRest";
    case (NonTermPow):
        return "Pow";
    case (NonTermPowRest):
        return "PowRest";
    case (NonTermAtom):
        return "Atom";
    case (NonTermArray):
        return "Array";
    case (NonTermItemsArray):
        return "ItemsArray";
    case (NonTermItemsArrayTail):
        return "ItemsArrayTail";
    case (NonTermExprBool):
        return "ExprBool";
    case (NonTermExprBoolOr):
        return "ExprBoolOr";
    case (NonTermExprBoolOrRest):
        return "ExprBoolOrRest";
    case (NonTermExprBoolAnd):
        return "ExprBoolAnd";
    case (NonTermExprBoolAndTail):
        return "ExprBoolAndTail";
    case (NonTermExprBoolNot):
        return "ExprBoolNot";
    case (NonTermExprBoolRel):
        return "ExprBoolRel";
    case (NonTermExprBoolRelTail):
        return "ExprBoolRelTail";
    case (NonTermExprBoolRelFactor):
        return "ExprBoolRelFactor";
    case (NonTermOpBool):
        return "OpBool";
    case (NonTermWhileStatement):
        return "WhileStatement";
    case (NonTermIfStatement):
        return "IfStatement";
    case (NonTermElseifStatementList):
        return "ElseifStatementList";
    case (NonTermElseifStatement):
        return "ElseifStatement";
    case (NonTermElseifStatementTail):
        return "ElseifStatementTail";
    case (NonTermElseStatement):
        return "ElseStatement";
    case (NonTermStatementStructure):
        return "StatementStructure";
    case (NonTermForStatement):
        return "ForStatement";
    case (NonTermForExprBool):
        return "ForExprBool";
    case (NonTermAssignExprList):
        return "AssignExprList";
    case (NonTermAssignExprTail):
        return "AssignExprTail";
    case (NonTermIdentifier):
        return "Identifier";
    case (NonTermIdentifierArray):
        return "IdentifierArray";
    default:
        return NULL;
    }
}

char *non_terminal_enum_to_str(int index)
{
    switch (index)
    {
    case (NonTermProgram):
        return "Program";
    case (NonTermCompoundStatement):
        return "CompoundStatement";
    case (NonTermStatementList):
        return "StatementList";
    case (NonTermStatementListTail):
        return "StatementListTail";
    case (NonTermStatement):
        return "Statement";
    case (NonTermExpr):
        return "Expr";
    case (NonTermReassignExpr):
        return "ReassignExpr";
    case (NonTermDeclExpr):
        return "DeclExpr";
    case (NonTermDeclExprAssign):
        return "DeclExprAssign";
    case (NonTermReassignExprTail):
        return "ReassignExprTail";
    case (NonTermArithExpr):
        return "ArithExpr";
    case (NonTermArithExprTail):
        return "ArithExprTail";
    case (NonTermArithExprTailRest):
        return "ArithExprTailRest";
    case (NonTermTerm):
        return "Term";
    case (NonTermTermList):
        return "TermList";
    case (NonTermTermListTail):
        return "TermListTail";
    case (NonTermFactor):
        return "Factor";
    case (NonTermFactorRest):
        return "FactorRest";
    case (NonTermPow):
        return "Pow";
    case (NonTermPowRest):
        return "PowRest";
    case (NonTermAtom):
        return "Atom";
    case (NonTermArray):
        return "Array";
    case (NonTermItemsArray):
        return "ItemsArray";
    case (NonTermItemsArrayTail):
        return "ItemsArrayTail";
    case (NonTermExprBool):
        return "ExprBool";
    case (NonTermExprBoolOr):
        return "ExprBoolOr";
    case (NonTermExprBoolOrRest):
        return "ExprBoolOrRest";
    case (NonTermExprBoolAnd):
        return "ExprBoolAnd";
    case (NonTermExprBoolAndTail):
        return "ExprBoolAndTail";
    case (NonTermExprBoolNot):
        return "ExprBoolNot";
    case (NonTermExprBoolRel):
        return "ExprBoolRel";
    case (NonTermExprBoolRelTail):
        return "ExprBoolRelTail";
    case (NonTermExprBoolRelFactor):
        return "ExprBoolRelFactor";
    case (NonTermOpBool):
        return "OpBool";
    case (NonTermWhileStatement):
        return "WhileStatement";
    case (NonTermIfStatement):
        return "IfStatement";
    case (NonTermElseifStatementList):
        return "ElseifStatementList";
    case (NonTermElseifStatement):
        return "ElseifStatement";
    case (NonTermElseifStatementTail):
        return "ElseifStatementTail";
    case (NonTermElseStatement):
        return "ElseStatement";
    case (NonTermStatementStructure):
        return "StatementStructure";
    case (NonTermForStatement):
        return "ForStatement";
    case (NonTermForExprBool):
        return "ForExprBool";
    case (NonTermAssignExprList):
        return "AssignExprList";
    case (NonTermAssignExprTail):
        return "AssignExprTail";
    case (NonTermIdentifier):
        return "Identifier";
    case (NonTermIdentifierArray):
        return "IdentifierArray";
    default:
        return NULL;
    }
}

int terminal_str_to_enum(const char *str)
{
    if (strcmp(str, "ε") == 0)
        return TermEpsilon;
    if (strcmp(str, "IDENTIFIER") == 0)
        return TermIdentifier;
    if (strcmp(str, "let") == 0)
        return TermDefVar;
    if (strcmp(str, "for") == 0)
        return TermFor;
    if (strcmp(str, "if") == 0)
        return TermIf;
    if (strcmp(str, "while") == 0)
        return TermWhile;
    if (strcmp(str, "{") == 0)
        return TermLbrace;
    if (strcmp(str, "}") == 0)
        return TermRbrace;
    if (strcmp(str, "(") == 0)
        return TermLparen;
    if (strcmp(str, ")") == 0)
        return TermRparen;
    if (strcmp(str, "$") == 0)
        return TermEof;
    if (strcmp(str, "=") == 0)
        return TermAssign;
    if (strcmp(str, ";") == 0)
        return TermSemi;
    if (strcmp(str, "-") == 0)
        return TermMinus;
    if (strcmp(str, "+") == 0)
        return TermPlus;
    if (strcmp(str, "FLOAT") == 0)
        return TermFloat;
    if (strcmp(str, "STRING") == 0)
        return TermString;
    if (strcmp(str, "INTEGER") == 0)
        return TermInteger;
    if (strcmp(str, "[") == 0)
        return TermLbracket;
    if (strcmp(str, "]") == 0)
        return TermRbracket;
    if (strcmp(str, "||") == 0)
        return TermOr;
    if (strcmp(str, "&&") == 0)
        return TermAnd;
    if (strcmp(str, ",") == 0)
        return TermComma;
    if (strcmp(str, "<=") == 0)
        return TermLe;
    if (strcmp(str, "<") == 0)
        return TermLt;
    if (strcmp(str, ">=") == 0)
        return TermGe;
    if (strcmp(str, ">") == 0)
        return TermGt;
    if (strcmp(str, "!=") == 0)
        return TermNe;
    if (strcmp(str, "==") == 0)
        return TermEq;
    if (strcmp(str, "*") == 0)
        return TermMul;
    if (strcmp(str, "/") == 0)
        return TermDiv;
    if (strcmp(str, "^") == 0)
        return TermPow;
    if (strcmp(str, "!") == 0)
        return TermNot;
    if (strcmp(str, "elseif") == 0)
        return TermElseIf;
    if (strcmp(str, "else") == 0)
        return TermElse;

    return -1;
}

int non_terminal_str_to_enum(const char *str)
{
    if (strcmp(str, "Program") == 0)
        return NonTermProgram;
    if (strcmp(str, "CompoundStatement") == 0)
        return NonTermCompoundStatement;
    if (strcmp(str, "StatementList") == 0)
        return NonTermStatementList;
    if (strcmp(str, "StatementListTail") == 0)
        return NonTermStatementListTail;
    if (strcmp(str, "Statement") == 0)
        return NonTermStatement;
    if (strcmp(str, "Expr") == 0)
        return NonTermExpr;
    if (strcmp(str, "ReassignExpr") == 0)
        return NonTermReassignExpr;
    if (strcmp(str, "DeclExpr") == 0)
        return NonTermDeclExpr;
    if (strcmp(str, "DeclExprAssign") == 0)
        return NonTermDeclExprAssign;
    if (strcmp(str, "ReassignExprTail") == 0)
        return NonTermReassignExprTail;
    if (strcmp(str, "ArithExpr") == 0)
        return NonTermArithExpr;
    if (strcmp(str, "ArithExprTail") == 0)
        return NonTermArithExprTail;
    if (strcmp(str, "ArithExprTailRest") == 0)
        return NonTermArithExprTailRest;
    if (strcmp(str, "Term") == 0)
        return NonTermTerm;
    if (strcmp(str, "TermList") == 0)
        return NonTermTermList;
    if (strcmp(str, "TermListTail") == 0)
        return NonTermTermListTail;
    if (strcmp(str, "Factor") == 0)
        return NonTermFactor;
    if (strcmp(str, "FactorRest") == 0)
        return NonTermFactorRest;
    if (strcmp(str, "Pow") == 0)
        return NonTermPow;
    if (strcmp(str, "PowRest") == 0)
        return NonTermPowRest;
    if (strcmp(str, "Atom") == 0)
        return NonTermAtom;
    if (strcmp(str, "Array") == 0)
        return NonTermArray;
    if (strcmp(str, "ItemsArray") == 0)
        return NonTermItemsArray;
    if (strcmp(str, "ItemsArrayTail") == 0)
        return NonTermItemsArrayTail;
    if (strcmp(str, "ExprBool") == 0)
        return NonTermExprBool;
    if (strcmp(str, "ExprBoolOr") == 0)
        return NonTermExprBoolOr;
    if (strcmp(str, "ExprBoolOrRest") == 0)
        return NonTermExprBoolOrRest;
    if (strcmp(str, "ExprBoolAnd") == 0)
        return NonTermExprBoolAnd;
    if (strcmp(str, "ExprBoolAndTail") == 0)
        return NonTermExprBoolAndTail;
    if (strcmp(str, "ExprBoolNot") == 0)
        return NonTermExprBoolNot;
    if (strcmp(str, "ExprBoolRel") == 0)
        return NonTermExprBoolRel;
    if (strcmp(str, "ExprBoolRelTail") == 0)
        return NonTermExprBoolRelTail;
    if (strcmp(str, "ExprBoolRelFactor") == 0)
        return NonTermExprBoolRelFactor;
    if (strcmp(str, "OpBool") == 0)
        return NonTermOpBool;
    if (strcmp(str, "WhileStatement") == 0)
        return NonTermWhileStatement;
    if (strcmp(str, "IfStatement") == 0)
        return NonTermIfStatement;
    if (strcmp(str, "ElseifStatementList") == 0)
        return NonTermElseifStatementList;
    if (strcmp(str, "ElseifStatement") == 0)
        return NonTermElseifStatement;
    if (strcmp(str, "ElseifStatementTail") == 0)
        return NonTermElseifStatementTail;
    if (strcmp(str, "ElseStatement") == 0)
        return NonTermElseStatement;
    if (strcmp(str, "StatementStructure") == 0)
        return NonTermStatementStructure;
    if (strcmp(str, "ForStatement") == 0)
        return NonTermForStatement;
    if (strcmp(str, "ForExprBool") == 0)
        return NonTermForExprBool;
    if (strcmp(str, "AssignExprList") == 0)
        return NonTermAssignExprList;
    if (strcmp(str, "AssignExprTail") == 0)
        return NonTermAssignExprTail;
    if (strcmp(str, "Identifier") == 0)
        return NonTermIdentifier;
    if (strcmp(str, "IdentifierArray") == 0)
        return NonTermIdentifierArray;

    return -1;
}

void error_expected_non_term(int in)
{
    printf("Expected: ");

    printf("\n\n");
}