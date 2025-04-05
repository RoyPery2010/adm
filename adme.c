#include "adme.h"

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: %s <file_name.adm>\n", argv[0]);
        exit(1);
    }
    char *file_name = argv[1];
    Machine *machine = malloc(sizeof(Machine));
    read_program_from_file(machine, file_name);
    printf("%zu\n", machine->program_size);
    run_instructions(machine);
    print_stack(machine);
}