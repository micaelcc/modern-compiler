#include "input-generator.h"
#define MAX_CODE_SIZE 2147483647
char *INSTRUCTIONS[] = {
    "let a = b + c * 5 / 10 * (4 + 5);",
    "let a = b + c * 5 / 10 * (4 + 5);",
    "a = d + c * 5 / 10 * (c + b);",
    "let result = 42;",
    "x = y - 2 * (z + 1);",
    "let x = (a + b) * c;",
    NULL
};

const char* random_instr() {
    int count = 0;
    while (INSTRUCTIONS[count]) count++;
    return INSTRUCTIONS[rand() % count];
}

void append_instr(char *dest, const char *instr) {
    strcat(dest, instr);
}

void generate_block(char *dest, int max_instrs) {
    for (int i = 0; i < max_instrs; i++) {
        append_instr(dest, random_instr());
        int choice = rand() % 4;
        switch (choice) {
            case 0:
            case 1:
                append_instr(dest, random_instr());
                break;
            case 2:
                strcat(dest, "if (a > b) {");
                append_instr(dest, random_instr());
                strcat(dest, "}");
                break;
            case 3:
                strcat(dest, "for (i = 0; i < 10; i = i + 1) {");
                append_instr(dest, random_instr());
                strcat(dest, "}");
                break;
        }
    }
}

char* generate(size_t max_top_instructions) {
    char *code = (char*)malloc(MAX_CODE_SIZE);
    if (!code) return NULL;

    code[0] = '\0';

    for (size_t i = 0; i < max_top_instructions; i++) {
        int choice = rand() % 5;
        switch (choice) {
            case 0:
            case 1:
            case 2:
                append_instr(code, random_instr());
                break;
            case 3:
                strcat(code, "if (a > b) {");
                generate_block(code, rand() % 10 + 1);
                strcat(code, "}");
                break;
            case 4:
                strcat(code, "for (i = 0; i < 10; i = i + 1) {");
                generate_block(code, rand() % 10 + 1);
                strcat(code, "}");
                break;
        }
    }

    return code;
}


