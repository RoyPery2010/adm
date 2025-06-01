#define ADM_IMPLEMENTATION
#include "./adm.h"
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./admi <input.adm>\n");
        fprintf(stderr, "expected input\n");
        exit(1);
    }

    adm_load_program_to_file(&adm, argv[1]);
    for (int i = 0; i < 69 && !adm.halt; ++i) {
        Err err = adm_execute_inst(&adm);
        adm_dump_stack(stdout, &adm);
        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            exit(1);
        }
    }
    return 0;
}