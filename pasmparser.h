#pragma once

#include <stdio.h>
#include "pasmlexer.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
typedef struct ParseList {
    Token value;
    struct ParseList *next;
} ParseList;

void print_list(ParseList *head);

ParseList parser(Lexer lexer);