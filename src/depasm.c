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

    for (Inst_Addr i = 0; i < adm.program_size; ++i) {
            printf("%s", inst_name(adm.program[i].type));
            if (inst_has_operand(adm.program[i].type)) {
                printf(" %ld", adm.program[i].operand.as_i64);
            }
            printf("\n");
    }
    return 0;
}