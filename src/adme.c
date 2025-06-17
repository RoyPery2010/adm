#define ADM_IMPLEMENTATION
#include "./adm.h"
#include <inttypes.h>

static char *shift(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

static void usage(FILE *stream, const char *program) {
    fprintf(stream, "Usage: %s -i <input.adm> -l <limit> [-h] [-d]\n", program);
}

Err adm_alloc(ADM *adm) {
    if (adm->stack_size < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    adm->stack[adm->stack_size - 1].as_ptr = malloc(adm->stack[adm->stack_size - 1].as_u64);

    return ERR_OK;
}

static Err adm_free(ADM *adm) {
    if (adm->stack_size < 1) {
        return ERR_STACK_UNDERFLOW;
    }

    free(adm->stack[adm->stack_size - 1].as_ptr);
    adm->stack_size -= 1;
    return ERR_OK;

}

int main(int argc, char **argv) {
    const char *program = shift(&argc, &argv); // skip the program name
    const char *input_file_path = NULL;
    int limit = -1;
    int debug = 0;

    while(argc > 0) {
        const char *flag = shift(&argc, &argv);
        if (strcmp(flag, "-i") == 0) {
            if (argc == 0) {
                usage(stderr, program);
                fprintf(stderr, "ERROR: No arguments is provided for flag `%s`\n", flag);
                exit(1);
            }
            input_file_path = shift(&argc, &argv);
        } else if (strcmp(flag, "-l") == 0) {
            if (argc == 0) {
                usage(stderr, program);
                fprintf(stderr, "ERROR: No arguments is provided for flag `%s`\n", flag);
                exit(1);
            }

            limit = atoi(shift(&argc, &argv));
        } else if (strcmp(flag, "-h") == 0) {
            usage(stdout, program);
            exit(0);
        } else if (strcmp(flag, "-d") == 0) {
            debug = 1;
        } else {
            usage(stderr, program);
            fprintf(stderr, "ERROR: Unknown flag `%s`\n", flag);
            exit(1);
        }
    }
    if (input_file_path == NULL) {
        usage(stderr, program);
        fprintf(stderr, "ERROR: input was not provided\n");
        exit(1);
    }
    adm_load_program_from_file(&adm, input_file_path);
    adm_push_native(&adm, adm_alloc);
    adm_push_native(&adm, adm_free);
    if (!debug) {
        Err err = adm_execute_program(&adm, limit);
        adm_dump_stack(stdout, &adm);
        if (err != ERR_OK) {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            return 1;
        }
    } else {
        while (limit != 0 && !adm.halt) {
            adm_dump_stack(stdout, &adm);
            getchar();
            printf("%s %" PRIu64 "\n", inst_name(adm.program[adm.ip].type), adm.program[adm.ip].operand.as_u64);
            Err err = adm_execute_inst(&adm);
            if (err != ERR_OK) {
                fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
                return 1;
            }

            if (limit > 0) {
                --limit;
            }
        }
    }
    
    return 0;
}