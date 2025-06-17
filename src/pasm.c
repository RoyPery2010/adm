#define ADM_IMPLEMENTATION
#include "./adm.h"
#include <assert.h>
#include <stdlib.h>

static char *shift(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

static void usage(FILE *stream, const char *program) {
    fprintf(stream, "Usage: %s <input.pasm> <output.adm>\n", program);
}

int main(int argc, char **argv) {
    char *program = shift(&argc, &argv); // Skip program name

    if (argc < 2) {
        usage(stderr, program);
        fprintf(stderr, "error: expected input and output files\n");
        return 1;
    }

    const char *input_file_path  = shift(&argc, &argv);
    const char *output_file_path = shift(&argc, &argv);

    String_View source = slurp_file(input_file_path);
    printf("Pasm: %.*s count=%d\n", (int)source.count, source.data, (int)source.count);
    adm_translate_source(source, &adm, &pasm);

    if (adm.program_size == 0) {
        fprintf(stderr, "error: failed to assemble program from '%s'\n", input_file_path);
        return 1;
    }

    adm_save_program_to_file(&adm, output_file_path);
    printf("Assembled %zu instructions from %s → %s\n", adm.program_size, input_file_path, output_file_path);
    return 0;
}
