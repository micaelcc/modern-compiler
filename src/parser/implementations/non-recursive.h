#include <stdbool.h>
#include <glib.h>
#include "../table/table.h"
#include "../../ast/ast.h"
#include "../../utils/string-utils.h"
#include "../../token/token.h"
#include "../../lex/lex.h"

typedef struct
{
    bool _let;
    bool _reassign;
    bool _array;
    bool _if;
    bool _for;
    bool _elseif;
    bool _else;
    bool _while;
    bool _for_initial_assignments;
    bool _for_condition;
    bool _for_final_assignments;
} IsParsingFlags;

ASTNode *parser_non_recurs();
int precedence(TokenSubtype);
bool is_high_precedence(Token, gpointer);
void perform_ast_node(int, Token, gpointer, gpointer);
void handle_ast(int, Token, Token *, IsParsingFlags *, gpointer, gpointer, gpointer, ASTNode *, ASTNode *);
