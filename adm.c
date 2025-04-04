#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./adm.h"

#define PROGRAM_SIZE sizeof(program)/sizeof(program[0])

Inst program[] = {
    DEF_INST_PUSH(1),
    DEF_INST_PUSH(4),
    DEF_INST_PUSH(6),
    DEF_INST_PUSH(8),
    DEF_INST_PUSH(10),
    DEF_INST_PUSH(12),
    DEF_INST_INDUP(2),
};


void push(Machine *machine, int value){
    if (machine->stack_size >= MAX_STACK_SIZE){
        fprintf(stderr, "ERROR: Stack Overflow\n");
        exit(1);
    }
    machine->stack_size++;
    //printf("stack size after push %d\n",  machine->stack_size);
    machine->stack[machine->stack_size-1] = value;
}

int pop(Machine *machine){
    if (machine->stack_size <= 0){
        fprintf(stderr, "ERROR: Stack Underflow\n");
        exit(1);
    }
    machine->stack_size--;
    return machine->stack[machine->stack_size];
}

void index_swap(Machine *machine, int index){
    if(index > machine->stack_size || index < 0){
        fprintf(stderr, "ERROR: Index out of range\n");
        exit(1);
    }
    int temp_value = machine->stack[index];
    machine->stack[index] = pop(machine);
    push(machine, temp_value);
}
void index_dup(Machine *machine, int index){
    if(index > machine->stack_size || index < 0){
        fprintf(stderr, "ERROR: Index out of range\n");
        exit(1);
    }
    push(machine, machine->stack[index]);
}

void print_stack(Machine *machine){
    printf("------ STACK\n");
    for (int i = machine->stack_size - 1; i >= 0; i--){
        printf("%d\n", machine->stack[i]);
    }
    printf("------ END OF STACK\n");
}

void write_program_to_file(Machine *machine, char *file_path){
    FILE *file = fopen(file_path, "wb");
    if (file == NULL){
        fprintf(stderr, "ERROR: Could not write to file %s\n", file_path);
        exit(1);
    }

    fwrite(machine->instructions, sizeof(machine->instructions[0]), machine->program_size, file);

    fclose(file);
}

Machine *read_program_from_file(Machine *machine, char *file_path){
    FILE *file = fopen(file_path, "rb");
    if (file == NULL){
        fprintf(stderr, "ERROR: Could not read from file %s\n", file_path);
        exit(1);
    }
    Inst *instructions = malloc(sizeof(Inst) * MAX_STACK_SIZE);
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(instructions, sizeof(instructions[0]), length / 8, file);

    machine->program_size = length / 8;
    printf("PROGRAM SIZE %zu, length: %d, %ld\n", sizeof(&instructions)/sizeof(instructions[0]), length/8, PROGRAM_SIZE);
    machine->instructions = instructions;
    fclose(file);
    return machine;
}

void run_instructions(Machine *machine){
    int a, b;
    printf("RUNNING\n");
    printf("program_size %lu\n", machine->program_size);
    for (size_t ip = 0; ip < machine->program_size; ip++){
        switch(machine->instructions[ip].type){
            case INST_PUSH:
                //fprintf(stdout, "push %d\n", machine->instructions[ip].value);
                push(machine, machine->instructions[ip].value);
                break;
            case INST_NOP:
                continue;
                break;
            case INST_POP:
                //fprintf(stdout, "pop\n"); 
                pop(machine);
                break;
            case INST_DUP:
                //fprintf(stdout, "dup\n"); 
                a = pop(machine);
                push(machine, a);
                push(machine, a);
                break;
            case INST_INDUP:
                index_dup(machine, machine->instructions[ip].value);
                break;
            case INST_SWAP:
                //fprintf(stdout, "swap\n"); 
                a = pop(machine);
                b = pop(machine);
                push(machine, a);
                push(machine, b);
                break;
            case INST_INSWAP:
                index_swap(machine, machine->instructions[ip].value);
                break;
            case INST_ADD:
                //fprintf(stdout, "add\n"); 
                a = pop(machine);
                b = pop(machine);
                push(machine, a + b);
                break;
            case INST_SUB:
                //fprintf(stdout, "sub\n"); 
                a = pop(machine);
                b = pop(machine);
                push(machine, a - b);
                break;
            case INST_MUL:
                //fprintf(stdout, "mul\n"); 
                a = pop(machine);
                b = pop(machine);
                push(machine, a * b);
                break;
            case INST_DIV:
                //fprintf(stdout, "div\n"); 
                a = pop(machine);
                b = pop(machine);
                if (b == 0){
                    fprintf(stderr, "ERROR: Cannot divide by 0\n");
                    exit(1);
                }
                push(machine, a / b);
                break;
            case INST_MOD:
                //fprintf(stdout, "div\n"); 
                a = pop(machine);
                b = pop(machine);
                push(machine, a % b);
                break;
            case INST_CMPE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a == b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPNE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a != b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPG:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a > b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPL:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a < b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPGE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a >= b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPLE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a <= b){
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_JMP:
                ip = machine->instructions[ip].value-1;
                if (ip >= machine->program_size){
                    printf("ip %ld,program size %lu\n ", ip, machine->program_size);
                    fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                    exit(1);
                }
                break;
            case INST_ZJMP:
                if (pop(machine) == 0){
                    ip = machine->instructions[ip].value-1;
                    if (ip >= machine->program_size){
                        fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                        exit(1);
                    }
                } else {
                    continue;
                }
                break;
            case INST_NZJMP:
                if (pop(machine) != 0){
                    ip = machine->instructions[ip].value-1;
                    if (ip >= machine->program_size){
                        fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                        exit(1);
                    }
                } else {
                    continue;
                }
                break;
            case INST_PRINT:
                //fprintf(stdout, "print\n"); 
                printf("%d\n", pop(machine));
                break;
            case INST_HALT:
                ip = machine->program_size;
                break;
        }
        print_stack(machine);
    }
}

int adm(){
    lexer();
    Machine *loaded_machine = malloc(sizeof(Machine));
    loaded_machine->instructions = program;
    write_program_to_file(loaded_machine, "test.adm");
    loaded_machine = read_program_from_file(loaded_machine, "test.adm");
    run_instructions(loaded_machine);
    print_stack(loaded_machine);
    return 0;
}