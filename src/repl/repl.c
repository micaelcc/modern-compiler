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
        ASTNode* node = execute_recursive_descendent();

        if (node != NULL) {
            print_ast(node, 0, 1);
        }

    }
}
