#include "repl.h"

char* generate_code(int n) {
    char buffer[250];
    char *generate_code_command = (char*)"bash generate_input.sh --max-tokens";
    sprintf(buffer, "%s %d", generate_code_command, n);
    
    FILE *fp = popen(buffer, "r");

    size_t buff_size = 512;
    char *file_buffer = (char*) malloc(sizeof(char) * buff_size);
    file_buffer[0] = '\0';
    size_t total_read = 0;

    while (fgets(file_buffer + total_read, buff_size - total_read, fp) != NULL) {
        total_read = strlen(file_buffer);
        if (total_read >= buff_size - 1) {
            break;
        }
    }
    pclose(fp);

    file_buffer[strlen(file_buffer)-1] = '\0';
    return file_buffer;
}

void print_samples()
{
    printf("Samples: ");
    printf("\n\tlet num = 10\n");
    printf("\tlet ufc = \"ufc\n");
    printf("\t:g 1 (0 - 10 tokens, 1 - 20 tokens, 2 - 30 tokens)\n");
    printf("\t:t\n");
    printf("\n\n");
}

void prompt(CompilerOptions options)
{
    printf("Type :s for an example, :g to generate input, :q to quit\n");
    srand(time(NULL));

    if (options.EXECUTE_TABLE_DRIVEN)
    {
        load_file();
        set_table();
    }
    for (;;)
    {
        printf("\n>>> ");

        char *s = input_string();

        if (s[0] == ':')
        {
            switch (s[1])
            {
            case 's':
                print_samples();
                continue;
            case 'g':
                s = generate_code(((s[3] - '0') + 1) * 10);
                printf(">>> %s\n", s);
                break;
            case 'q':
                free_table();
                free_tokens();
                exit(1);
            }
        }

        if (options.EXECUTE_BISON) {
            YY_BUFFER_STATE buffer = yy_scan_string(s);
            parse(options);

            yy_delete_buffer(buffer);
            continue;
        }
        
        make_tokens(s);

        if (options.PRINT_TOKENS)
        {
            printf("\033[38;5;202mStep[Lexer][%lu tokens] ->\033[0m", get_number_of_tokens());
            print_tokens(get_tokens());
        }

        if (options.PRINT_PARSER_RESULT)
        {
            printf("\033[38;5;202m\nStep[Parser] -> \033[0m");
        }

        ParseTree *tree = parse(options);

        if (options.PRINT_PARSE_TREE && tree != NULL)
        {
            print_tree(tree);
            free_parse_tree(tree);
        }

        printf("Tok[%d]\n\n", get_number_of_tokens());

        free_tokens();
    }
}
