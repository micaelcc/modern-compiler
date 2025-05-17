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
                break;
            case 'q':
                exit(1);
            }

            continue;
        }

        // char *ss = "let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;let a=[2,3,4,5];let i;let aux;for(aux=0,i=1;i<5;i=i+1){aux=a[i]+aux;}let new;if(aux>10){new=aux;}else {new=-10;}let a=[1,2,3,4];let b=[1,2,3]+a[1]+1;";

        if (strlen(s) > 2)
            make_tokens(s);
        else
            make_tokens(gerarCodigo(100000));

        get_tokens();

        // printf("\033[38;5;202mStep[Lexer] ->\033[0m");
        // print_tokens(tokens);

        printf("\033[38;5;202m\nStep[Parser] -> \033[0m");

        execute_recursive_descendent();

        // execute_non_recursive();

        printf("Tokens: %lu\n", get_number_of_tokens());
    }
}
