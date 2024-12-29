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
    printf("Type :s for an example, :q to quit\n");

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
                break;
            case 'q':
                exit(1);
            }

            continue;
        }

        make_tokens(s);

        Token* tokens = get_tokens();
        printf("\033[38;5;202mStep[Lexer] ->\033[0m");
        print_tokens(tokens);

        printf("\033[38;5;202m\nStep[Parser] -> \033[0m");
        PARSER_ERROR = 0;
        execute_recursive_descendent();

        if (!PARSER_ERROR) printf("\033[0;32m\t\tOk\033[0m\n");

    }
}
