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
        
        printf("Step[Lexer] -> ");
        print_tokens(tokens);
        free(tokens);
    }
}
