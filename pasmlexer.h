#pragma once
#include "./adm.h"


#define MAX_TOKEN_STACK_SIZE 1024

typedef enum {
    TYPE_NONE = -1,
    TYPE_NOP = 0,
    TYPE_PUSH,
    TYPE_POP,
    TYPE_DUP,
    TYPE_INDUP,
    TYPE_SWAP,
    TYPE_INSWAP,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_MUL,
    TYPE_DIV,
    TYPE_MOD,
    TYPE_CMPE,
    TYPE_CMPNE,
    TYPE_CMPG,
    TYPE_CMPL,
    TYPE_CMPGE,
    TYPE_CMPLE,
    TYPE_JMP,
    TYPE_ZJMP,
    TYPE_NZJMP,
    TYPE_PRINT,
    TYPE_INT,
    TYPE_HALT,
} TokenType;

typedef struct {
    TokenType type;
    char *text;
    int line;
    int character;
} Token;

typedef struct {
    Token token_stack[MAX_TOKEN_STACK_SIZE];
    int stack_size;
    char *file_name;
} Lexer;


Lexer lexer();
char *open_file(char *file_path, int *length);
void push_token(Lexer *lex, Token value);
Token pop_token(Lexer *lex);
Token init_token(TokenType type, char *text, int line, int character);
TokenType check_builtin_keywords(char *name);
void print_token(Token token);
Token generate_keyword(char *current, int *current_index, int line, int character);
Token generate_int(char *current, int *current_index, int line, int character);
