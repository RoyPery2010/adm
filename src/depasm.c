#define ADM_IMPLEMENTATION
#include "./adm.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./depasm <input.adm>\n");
        fprintf(stderr, "no input file was provided\n");
        exit(1);
    }

    const char *input_file_path = argv[1];
    adm_load_program_from_file(&adm, input_file_path);

    for (Word i = 0; i < adm.program_size; ++i) {
        switch (adm.program[i].type) {
            case INST_NOP:
                printf("nop\n");
                break;
            case INST_PUSH:
                printf("push %ld\n", adm.program[i].operand);
                break;
            case INST_DUP:
                printf("dup %ld\n", adm.program[i].operand);
                break;
            case INST_PLUS:
                printf("plus\n");
                break;
            case INST_MINUS:
                printf("minus\n");
                break;
            case INST_MULT:
                printf("mult\n");
                break;
            case INST_DIV:
                printf("div\n");
                break;
            case INST_JMP:
                printf("jmp %ld\n", adm.program[i].operand);
                break;
            case INST_JMP_IF:
                printf("jmp_if %ld\n", adm.program[i].operand);
                break;
            case INST_EQ:
                printf("eq\n");
                break;
            case INST_HALT:
                printf("halt\n");
                break;
            case INST_PRINT_DEBUG:
                printf("print_debug\n");
                break;
        }
    }
}