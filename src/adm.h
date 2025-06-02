#ifndef ADM_H
#define ADM_H



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>



#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
#define ADM_STACK_CAPACITY 1024
#define ADM_PROGRAM_CAPACITY 1024
#define LABEL_CAPACITY 1024
#define DEFERRED_OPERANDS_CAPACITY 1024

#define MAKE_INST_PUSH(value) {.type = INST_PUSH, .operand = (value)}
#define MAKE_INST_PLUS {.type = INST_PLUS}
#define MAKE_INST_MINUS {.type = INST_MINUS}
#define MAKE_INST_MULT {.type = INST_MULT}
#define MAKE_INST_DIV {.type = INST_DIV}
#define MAKE_INST_JMP(addr) {.type = INST_JMP, .operand = (addr)}
#define MAKE_INST_HALT {.type = INST_HALT, .operand = 0}
#define MAKE_INST_JMP_IF(addr) {.type = INST_JMP_IF, .operand = (addr)}
#define MAKE_INST_EQ {.type = INST_EQ}
#define MAKE_INST_PRINT_DEBUG {.type = INST_PRINT_DEBUG, .operand = 0}
#define MAKE_INST_DUP(addr) {.type = INST_DUP, .operand = (addr)}

typedef uint64_t Inst_Addr;

typedef union {
    uint64_t as_u64;
    int64_t as_i64;
    double as_f64;
    void *as_ptr;
} Word;

static_assert(sizeof(Word) == 8, "The ADM's Word is expected to be 64 bits");


typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_ILLEGAL_INST,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND,
    ERR_DIV_BY_ZERO,
} Err;

const char *err_as_cstr(Err err);

typedef enum {
    INST_NOP = 0, //0
    INST_PUSH, //1
    INST_DUP, //2
    INST_PLUS, //3
    INST_MINUS, //4
    INST_MULT, //5
    INST_DIV, //6
    INST_JMP, //7
    INST_JMP_IF,
    INST_EQ,
    INST_HALT,
    INST_PRINT_DEBUG,
} Inst_Type;

typedef struct {
    Inst_Type type;
    Word operand;
} Inst;

typedef struct {
    Word stack[ADM_STACK_CAPACITY];
    uint64_t stack_size;
    Inst program[ADM_PROGRAM_CAPACITY];
    uint64_t program_size;
    Inst_Addr ip;
    int halt;
} ADM;



const char *inst_type_as_cstr(Inst_Type type);



Err adm_execute_program(ADM *adm, int limit);
Err adm_execute_inst(ADM *adm);

void adm_dump_stack(FILE *stream, const ADM *adm);


void adm_load_program_from_memory(ADM *adm, Inst *program, size_t program_size);

void adm_save_program_to_file(const ADM *adm, const char *file_path);
void adm_load_program_from_file(ADM *adm, const char *file_path);





typedef struct {
    size_t count;
    const char *data;
} String_View;

String_View cstr_as_sv(const char *cstr);
String_View sv_trim_left(String_View sv);

String_View sv_trim_right(String_View sv);

String_View sv_trim(String_View sv);
String_View sv_chop_by_delim(String_View *sv, char delim);
int sv_eq(String_View a, String_View b);
int sv_to_int(String_View sv);

String_View slurp_file(const char *file_path);

typedef struct {
    String_View name;
    Inst_Addr addr;
} Label;

typedef struct {
    Inst_Addr addr;
    String_View label;
} Deferred_Operand;

typedef struct {
    Label labels[LABEL_CAPACITY];
    size_t labels_size;
    Deferred_Operand deferred_operands[DEFERRED_OPERANDS_CAPACITY];
    size_t deferred_operands_size;
} Pasm;

Pasm pasm = {0};

Inst_Addr pasm_find_label_addr(Pasm *pasm, String_View name);
void pasm_push_label(Pasm *pasm, String_View name, Inst_Addr addr);
void pasm_push_deferred_operand(Pasm *pasm, Inst_Addr addr, String_View label);


void adm_translate_source(String_View source, ADM *adm, Pasm *lt);

#ifdef ADM_IMPLEMENTATION
ADM adm = {0};

const char *err_as_cstr(Err err) {
    switch (err) {
        case ERR_OK:
            return "ERR_OK";
        case ERR_STACK_OVERFLOW:
            return "ERR_STACK_OVERFLOW";
        case ERR_STACK_UNDERFLOW:
            return "ERR_STACK_UNDERFLOW";
        case ERR_ILLEGAL_INST:
            return "ERR_ILLEGAL_INST";
        case ERR_ILLEGAL_INST_ACCESS:
            return "ERR_ILLEGAL_INST_ACCESS";
        case ERR_ILLEGAL_OPERAND:
            return "ERR_ILLEGAL_OPERAND";
        case ERR_DIV_BY_ZERO:
            return "ERR_DIV_BY_ZERO";
        default:
            assert(0 && "err_as_cstr: Unreachable");
    }
    
}

const char *inst_type_as_cstr(Inst_Type type) {
    switch (type) {
        case INST_NOP: return "INST_NOP";
        case INST_PUSH: return "INST_PUSH";
        case INST_PLUS: return "INST_PLUS";
        case INST_MINUS: return "INST_MINUS";
        case INST_MULT: return "INST_MULT";
        case INST_DIV: return "INST_DIV";
        case INST_JMP: return "INST_JMP";
        case INST_JMP_IF: return "INST_JMP_IF";
        case INST_EQ: return "INST_EQ";
        case INST_HALT: return "INST_HALT";
        case INST_PRINT_DEBUG: return "INST_PRINT_DEBUG";
        case INST_DUP: return "INST_DUP";
        default: assert(0 && "inst_type_as_cstr: unreachable");
    }
}

Err adm_execute_program(ADM *adm, int limit) {
    //printf("Executing program with %zu instructions\n", adm->program_size);
    while (limit != 0 && !adm->halt) {
        //printf("Executing instruction at ip=%ld: %s\n", adm->ip, inst_type_as_cstr(adm->program[adm->ip].type));
        Err err = adm_execute_inst(adm);
        if (err != ERR_OK) {
            return err;
        }

        if (limit > 0) {
            --limit;
        }
    }
    return ERR_OK;
}

Err adm_execute_inst(ADM *adm) {

    if (adm->ip >= adm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }

    Inst inst = adm->program[adm->ip];

    switch (inst.type) {
        case INST_NOP:
            adm->ip += 1;
            break;
        case INST_PUSH:
            if (adm->stack_size >= ADM_STACK_CAPACITY) {
                return ERR_STACK_OVERFLOW;
            }
            adm->stack[adm->stack_size++] = inst.operand;
            adm->ip += 1;
            break;
        case INST_PLUS:
            if (adm->stack_size < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            adm->stack[adm->stack_size - 2].as_u64 += adm->stack[adm->stack_size - 1].as_u64;
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_MINUS:
            if (adm->stack_size < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            adm->stack[adm->stack_size - 2].as_u64 -= adm->stack[adm->stack_size - 1].as_u64;
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_MULT:
            if (adm->stack_size < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            adm->stack[adm->stack_size - 2].as_u64 *= adm->stack[adm->stack_size - 1].as_u64;
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_DIV:
            if (adm->stack_size < 2) {
                return ERR_STACK_UNDERFLOW;
            }

            if (adm->stack[adm->stack_size - 1].as_u64 == 0) {
                return ERR_DIV_BY_ZERO;
            }
            adm->stack[adm->stack_size - 2].as_u64 /= adm->stack[adm->stack_size - 1].as_u64;
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_JMP:
            adm->ip = inst.operand.as_u64;
            break;
        case INST_HALT:
            adm->halt = 1;
            break;
        case INST_EQ:
            if (adm->stack_size < 2) {
                return ERR_STACK_UNDERFLOW;
            }

            adm->stack[adm->stack_size - 2].as_u64 = adm->stack[adm->stack_size - 1].as_u64 == adm->stack[adm->stack_size - 2].as_u64;
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_JMP_IF:
            if (adm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }

            if (adm->stack[adm->stack_size - 1].as_u64) {
                adm->stack_size -= 1;
                adm->ip = inst.operand.as_u64;
            } else {
                adm->ip += 1;
            }
            break;
        case INST_PRINT_DEBUG:
            if (adm->stack_size < 1) {
                return ERR_STACK_UNDERFLOW;
            }

            printf("%lu\n", adm->stack[adm->stack_size - 1].as_u64);
            adm->stack_size -= 1;
            adm->ip += 1;
            break;
        case INST_DUP:
            if (adm->stack_size >= ADM_STACK_CAPACITY) {
                return ERR_STACK_OVERFLOW;
            }
            if (adm->stack_size - inst.operand.as_u64 <= 0) {
                return ERR_STACK_UNDERFLOW;
            }


            adm->stack[adm->stack_size] = adm->stack[adm->stack_size - 1 - inst.operand.as_u64];
            adm->stack_size += 1;
            adm->ip += 1;
            break;
        default:
            return ERR_ILLEGAL_INST;
        }
        return ERR_OK;
}

void adm_dump_stack(FILE *stream, const ADM *adm) {
    fprintf(stream, "Stack:\n");
    if (adm->stack_size > 0) {
        for (Inst_Addr i = 0; i < adm->stack_size; ++i) {
            fprintf(stream, " u64: %lu, i64: %ld, f64: %lf, ptr: %p\n", adm->stack[i].as_u64, 
                    adm->stack[i].as_i64, 
                    adm->stack[i].as_f64, 
                    adm->stack[i].as_ptr);
        }
    } else {
        fprintf(stream, "    [empty]\n");
    }
}


void adm_load_program_from_memory(ADM *adm, Inst *program, size_t program_size) {
    assert(program_size < ADM_PROGRAM_CAPACITY);
    memcpy(adm->program, program, sizeof(program[0]) * program_size);
    adm->program_size = program_size;
}

void adm_save_program_to_file(const ADM *adm, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    fwrite(adm->program, sizeof(Inst), adm->program_size, f);

    /*for (Word i = 0; i < adm->program_size; i++) {
        fprintf(f, "%d %ld\n", adm->program[i].type, adm->program[i].operand);
    }*/


    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    fclose(f);
}

void adm_load_program_from_file(ADM *adm, const char *file_path) {
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    long m = ftell(f);

    if (m < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }


    assert(m % sizeof(Inst) == 0);
    assert((size_t) m <= ADM_PROGRAM_CAPACITY * sizeof(Inst));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    rewind(f);

    adm->program_size = m / sizeof(Inst);
    fread(adm->program, sizeof(Inst), adm->program_size, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    /*adm->program_size = 0;
    while (fscanf(f, "%d %ld", (int*)&adm->program[adm->program_size].type, &adm->program[adm->program_size].operand) == 2) {
        adm->program_size++;
    }
    
    for (Word i = 0; i < adm->program_size; ++i) {
        printf("%s %ld\n", inst_type_as_cstr(adm->program[i].type), adm->program[i].operand);
    }*/

    fclose(f);
}









String_View cstr_as_sv(const char *cstr) {
    return (String_View) {
        .count = strlen(cstr),
        .data = cstr,
    };
}
String_View sv_trim_left(String_View sv) {
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i += 1;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data + i,
    };
}

String_View sv_trim_right(String_View sv) {
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count-1-i])) {
        i += 1;
    }
    return (String_View) {
        .count = sv.count - i,
        .data = sv.data,
    };
}

String_View sv_trim(String_View sv) {
    return sv_trim_right(sv_trim_left(sv));
}

String_View sv_chop_by_delim(String_View *sv, char delim) {
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }
    //printf("sv_chop_by_delim: i=%zu, count=%zu, delim='%c'\n", i, sv->count, delim);

    String_View result = {
        .count = i,
        .data = sv->data,
    };
    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data += i + 1;
    }/*/ else {
        sv->count -= i;
        sv->data += i;
    }*/
    return result;
}
int sv_eq(String_View a, String_View b) {
    if (a.count != b.count) {
        return 0;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}
int sv_to_int(String_View sv) {
    int result = 0;
    for (size_t i = 0; i < sv.count && isdigit(sv.data[i]); ++i) {
        result = result * 10 + sv.data[i] - '0';
    }
    return result;
}



Inst_Addr pasm_find_label_addr(Pasm *pasm, String_View name) {
    for (size_t i = 0; i < pasm->labels_size; ++i) {
        printf("label_table_find: i=%zu, name=%.*s\n", i, (int)pasm->labels[i].name.count, pasm->labels[i].name.data);
        if (sv_eq(pasm->labels[i].name, name)) {
            return pasm->labels[i].addr;
        }
    }
    fprintf(stderr, "ERROR: Label `%.*s` does not exists\n", (int)name.count, name.data);
    exit(1);
}

void pasm_push_label(Pasm *pasm, String_View name, Inst_Addr addr) {
    assert(pasm->labels_size < LABEL_CAPACITY);
    pasm->labels[pasm->labels_size++] = (Label) {
        .name = name,
        .addr = addr,
    };
}

void pasm_push_deferred_operand(Pasm *pasm, Inst_Addr addr, String_View label) {
    assert(pasm->deferred_operands_size < DEFERRED_OPERANDS_CAPACITY);
    pasm->deferred_operands[pasm->deferred_operands_size++] = (Deferred_Operand) {
        .addr = addr,
        .label = label,
    };
}



void adm_translate_source(String_View source, ADM *adm, Pasm *lt) {
    adm->program_size = 0;
    while (source.count > 0) {
        //if (adm->program_size >= 16) break;
        assert(adm->program_size < ADM_PROGRAM_CAPACITY);
        String_View line = sv_trim(sv_chop_by_delim(&source, '\n'));
        printf("Line 1: %.*s count=%d\n", (int)line.count, line.data, (int)line.count);
        if (line.count > 0 && *line.data != '#') {
            line = sv_trim(line);
        }
        //printf("Line 2: %.*s count=%d\n", (int)line.count, line.data, (int)line.count);
        if (line.count == 0 || line.data[0] == '#') {
            continue; // skip empty lines and comments
        }
        //printf("Line 3: %.*s count=%d\n", (int)line.count, line.data, (int)line.count);
   
        String_View original = line;

        String_View word = sv_chop_by_delim(&line, ' ');
        printf("Word: `%.*s` count=%d\n", (int)word.count, word.data, (int)word.count);
        if (word.count > 0 && word.data[word.count - 1] == ':') {
            String_View label = {
                .count = word.count - 1,
                .data = word.data,

            };
            word = sv_trim(sv_chop_by_delim(&line, ' '));
            printf("Label: %.*s %ld\n", (int)label.count, label.data, adm->program_size);
            pasm_push_label(lt, label, adm->program_size);
        }
        
        else if (sv_eq(word, cstr_as_sv("nop"))) {
            adm->program[adm->program_size++] = (Inst) {
                .type = INST_NOP
            };
        } else if (sv_eq(word, cstr_as_sv("push"))) {
            printf("Found push %.*s\n", (int)line.count, line.data);
            adm->program[adm->program_size++] = (Inst) {
                .type = INST_PUSH, 
                .operand = {.as_i64 = sv_to_int(sv_trim(line))}
            };
        } else if (sv_eq(word, cstr_as_sv("dup"))) {
            adm->program[adm->program_size++] = (Inst){
                .type = INST_DUP, 
                .operand = {.as_i64 = sv_to_int(sv_trim(line))}
            };
        } else if (sv_eq(word, cstr_as_sv("plus"))) {
            adm->program[adm->program_size++] = (Inst){
                .type = INST_PLUS
            };
        } else if (sv_eq(word, cstr_as_sv("jmp"))) {
            String_View labelString = sv_trim(line);
            int labelInt = sv_to_int(labelString);
            if (labelInt > 0 && isdigit(*labelString.data)) {
                adm->program[adm->program_size++] = (Inst){
                    .type = INST_JMP,
                    .operand = {.as_i64 = labelInt}, 
                };
                printf("Adding jmp to addr %d\n", labelInt);
            } else {
                pasm_push_deferred_operand(lt, adm->program_size, labelString);
                adm->program[adm->program_size++] = (Inst){
                    .type = INST_JMP
                };
                printf("Adding unresolved jmp to label %.*s at addr %ld\n", 
                       (int)labelString.count, labelString.data, adm->program_size);
            }
        } else {
                fprintf(stderr, "Unknown instruction: %.*s\n", (int)original.count, original.data);
                exit(1);
        }
    }
    for (size_t i = 0; i < pasm.deferred_operands_size; ++i) {
        Inst_Addr addr = pasm_find_label_addr(lt, pasm.deferred_operands[i].label);
        /*printf("Resolving unresolved jmp: %.*s at addr %ld to %ld\n", 
               (int)pasm->deferred_operands[i].label.count, 
               pasm->deferred_operands[i].label.data, 
               pasm->deferred_operands[i].addr, 
               addr);*/
        adm->program[pasm.deferred_operands[i].addr].operand.as_u64 = addr;
    }
    
    for (Inst_Addr i = 0; i < adm->program_size; ++i) {
        printf("%s %ld\n", inst_type_as_cstr(adm->program[i].type), adm->program[i].operand.as_i64);
    }
}

String_View slurp_file(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    long m = ftell(f);

    if (m < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }


    char *buffer = malloc(m);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for file: %s\n", strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    size_t n = fread(buffer, 1, m, f);
    
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    fclose(f);

    return (String_View) {
        .count = n,
        .data = buffer,
    };
}
#endif // ADM_IMPLEMENTATION

#endif // ADM_H