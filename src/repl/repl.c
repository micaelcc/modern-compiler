#include "repl.h"

void print_samples()
{
    printf("Samples: ");
    printf("\n\tlet num = 10\n");
    printf("\tlet ufc = \"ufc\"");
    printf("\n\n");
}

void prompt()
{
    printf("Type :s for an example, :g to generate input, :q to quit\n");
    srand(time(NULL));
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
                s = generate(20000);
                printf("%s\n", s);
                printf("Generated input: Array[%d]\n\n", strlen(s));
                break;
            case 'q':
                exit(1);
            }

            
        }

        make_tokens(s);

        if (PRINT_TOKENS) {
            printf("\033[38;5;202mStep[Lexer] ->\033[0m");
            print_tokens(get_tokens());
            printf("\t\tTokens: %lu\n", get_number_of_tokens());
        }

        printf("\033[38;5;202m\nStep[Parser] -> \033[0m");

        if (EXECUTE_RECURSIVE_DESCENT) {
            execute_recursive_descendent();
        }

        if (EXECUTE_TABLE_DRIVEN) { 
            execute_non_recursive();
        }
        
    }
}
