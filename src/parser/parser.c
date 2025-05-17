#include "parser.h"
extern int success;
extern size_t current_token_index;
const bool PRINT_AST = true;

ASTNode *execute_recursive_descendent()
{
    clock_t start, end;
    double execution_time;

    struct rusage usage_before, usage_after;

    start = clock();
    printf("\n\033\t\t[0;34mExecuting Recursive Descentent...\033[0m\n");
    getrusage(RUSAGE_SELF, &usage_before);

    ASTNode *ast_node = handle_program();

    getrusage(RUSAGE_SELF, &usage_after);

    end = clock();

    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    if (ast_node != NULL)
        printf("\033[0;32m\t\tSuccess!\033[0m\n");

    printf("\t\tExecution Time (ASDRP): %f milliseconds\n", execution_time);
    long memory_used2 = usage_after.ru_maxrss - usage_before.ru_maxrss;
    printf("\t\tMemory consumption (ASDRP): %ld KB\n", memory_used2);
    current_token_index = 0;

    if (PRINT_AST)
    {
        printf("\t\tAbstract Syntax Tree (AST): \n");
        print_ast(ast_node);
    }

    return ast_node;
}

ASTNode *execute_non_recursive()
{
    load_file();
    set_table();

    clock_t start, end;
    double execution_time;

    struct rusage usage_before, usage_after;

    printf("\033\n\n\t\t[0;34mExecuting Table-Driven Parser...\033[0m\n");
    getrusage(RUSAGE_SELF, &usage_before);
    start = clock();

    ASTNode *ast_node = parser_non_recurs();
    getrusage(RUSAGE_SELF, &usage_after);

    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    if (ast_node != NULL)
        printf("\033[0;32m\t\tSuccess!\033[0m\n");

    printf("\t\tExecution Time (ASTP): %f milliseconds\n", execution_time);

    long memory_used = usage_after.ru_maxrss - usage_before.ru_maxrss;
    printf("\t\tMemory consumption (ASTP): %ld KB\n", memory_used);
    current_token_index = 0;

    if (PRINT_AST)
    {
        printf("\t\tAbstract Syntax Tree (AST): \n");
        print_ast(ast_node);
    }

    return ast_node;
}
