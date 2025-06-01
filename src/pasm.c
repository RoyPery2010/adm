#define ADM_IMPLEMENTATION
#include "./adm.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: ./pasm <input.pasm> <output.adm>\n");
        fprintf(stderr, "expected input and output\n");
        exit(1);
    }

    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];
    String_View source = slurp_file(input_file_path);
    adm.program_size = adm_translate_source(source, adm.program, ADM_PROGRAM_CAPACITY);
    adm_save_program_to_file(adm.program, adm.program_size, output_file_path);
    return 0;
}