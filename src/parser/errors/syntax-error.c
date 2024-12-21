#include "syntax-error.h"

void error_handler(char error_msg[]) {
    char** error_log = get_current_line_error_log();

    printf("\nSyntax error: %s\n", error_msg);

    printf("\n\033[32m %s\033[0m\n", error_log[0]);
    printf("\033[34m %s\033[0m\n",error_log[1]);

    exit(1);
}

char** get_current_line_error_log() {
    size_t curr_tok_index = get_current_token_index();
    
    size_t first_tok_index = curr_tok_index;
    size_t last_tok_index = first_tok_index;
    size_t size = 0;
    Token* tokens = get_tokens();
        printf("%d %d\n", first_tok_index, last_tok_index);

    printf("%d\n", first_tok_index);
    while(first_tok_index >= 0 && tokens[first_tok_index].type != TOKEN_KEY && tokens[first_tok_index].type != TOKEN_SEMICOLON) {
        size += strlen(tokens[first_tok_index].value);
        first_tok_index--;
    }

    while(tokens[last_tok_index].type != TOKEN_EOF && tokens[last_tok_index].type != TOKEN_KEY && tokens[last_tok_index].type != TOKEN_SEMICOLON) {
        
        size += strlen(tokens[last_tok_index].value);
        last_tok_index++;
    }

    size += last_tok_index - first_tok_index + 1;

    if (tokens[last_tok_index].type != TOKEN_SEMICOLON) {
        size -= strlen(tokens[last_tok_index].value);
        last_tok_index--;
    }
    
    char *dest = malloc(sizeof(char) * size);
    char *aux = malloc(sizeof(char) * size);

    size_t error_index;

    for(; first_tok_index <= last_tok_index; first_tok_index++) {
        if (first_tok_index == curr_tok_index) 
            error_index = strlen(dest)-1;

        strcat(dest, tokens[first_tok_index].value);

        if(tokens[first_tok_index].type == TOKEN_KEY)
            strcat(dest, " ");
    }

    if (tokens[curr_tok_index].type == TOKEN_EOF) 
        error_index = strlen(dest) - 1;

    for(size_t i=0; i<strlen(dest); i++){
        if (i == error_index)
            aux[i] = '^';
        else 
            aux[i] = '~';
    }
    

    char** r = malloc(sizeof(char*) * 3);
    r[0] = dest;
    r[1] = aux;
    r[2] = NULL;

    return r;
}