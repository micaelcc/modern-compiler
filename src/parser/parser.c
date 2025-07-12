#include "parser.h"

long double getMemoryConsumption()
{
    char buffer[1024] = "";

    FILE *file = fopen("/proc/self/status", "r");
    if (!file)
    {
        printf("Call to getMemory FAILED; linux file proc/self/status not found!\n");
        return 1;
    }

    unsigned long currRealMemKB = 0;

    while (fscanf(file, " %1023s", buffer) == 1)
    {
        if (strcmp(buffer, "VmRSS:") == 0)
        {
            fscanf(file, " %lu", &currRealMemKB);
            break;
        }
    }

    fclose(file);

    return (long double)currRealMemKB;
}

char* parser_name(CompilerOptions op) {
    if (op.EXECUTE_BISON) return "Bison LR Parser";
    if (op.EXECUTE_RECURSIVE_DESCENT) return "Recursive Descent";
    if (op.EXECUTE_TABLE_DRIVEN) return "Table Driven";

    return "undef";
}

ParserBMark execute_parser_bmark(CompilerOptions opt)
{
    struct timespec start, end;
    struct mallinfo2 start_heap, end_heap;
    long double memory_consumption = 0, memory_heap_consumption = 0;
    long double start_memory, end_memory;
    long double execution_time = 0;
    size_t i;
    ParseTree *parse_tree_node = NULL;
    
    int REPS = opt.EXECUTE_BISON ? 1 : BMARK_REPS;

    for (i = 0; i < REPS; i++)
    {
        start_heap = mallinfo2();
        start_memory = getMemoryConsumption();
        clock_gettime(CLOCK_MONOTONIC, &start);
        parse_tree_node = parse(opt);
        clock_gettime(CLOCK_MONOTONIC, &end);
        end_memory = getMemoryConsumption();
        end_heap = mallinfo2();

        execution_time += (long double)(end.tv_sec - start.tv_sec) * 1000.0 +
                          (end.tv_nsec - start.tv_nsec) / 1e6;

        memory_consumption += (long double)end_memory - start_memory;
        memory_heap_consumption += (end_heap.uordblks - start_heap.uordblks) / 1024.0; //

        if (i < REPS - 1)
        {
            free_parse_tree(parse_tree_node);
        }

        sleep(0.5);
    }

    ParserBMark result = {
        .parser = parser_name(opt),
        .time_in_ms = execution_time / REPS,
        .memory_consumption_in_KB = memory_consumption / REPS,
        .memory_heap_consumption_in_KB = memory_heap_consumption / REPS,
        .tokens_analyzed = get_current_token_index(),
        .tree = parse_tree_node};

    return result;
}

ParseTree *parse(CompilerOptions options)
{
    ParseTree *tree = NULL;
    set_current_token_index(0);

    if (options.EXECUTE_TABLE_DRIVEN)
    {
        if (options.ONLY_SYNTAX_CHECK)
        {
            table_driven_parser_only_syntax_check();
        }
        else
        {
            return table_driven_parser();
        }
    }
    else if (options.EXECUTE_RECURSIVE_DESCENT)
    {
        if (options.ONLY_SYNTAX_CHECK)
        {
            handle_program_syntax_only();
        }
        else
        {
            return handle_program();
        }
    } else if (options.EXECUTE_BISON) {
        yyparse();
    }

    return tree;
}
