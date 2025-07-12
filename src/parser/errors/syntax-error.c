#include "syntax-error.h"
void error_handler(char error_msg[])
{
    printf("\033[0;31m\t\tSyntax error => %s\033[0m\n", error_msg);
}

void err_expected(char *expected, char *founded)
{
    printf("\033[0;31m\t\tSyntax error => Expected: %s, found: %s\033[0m\n", expected, founded);
}

void err_expected_term(ParseTable tbl, int non_term) {
    char *expected = (char*)malloc(sizeof(char) * 1000);
    int i;

    for(i = TermIdentifier; i < TermElse; i++) {
        if (tbl[non_term][i] != NULL) {
            strcat(expected, terminal_enum_to_str(i));
            strcat(expected, " | ");
        }
    }
    
    printf("\033[0;31m\t\tSyntax error => Expected: %s\033[0m\n", expected);

    free(expected);
}