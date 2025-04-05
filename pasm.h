#pragma once

#include "pasmlexer.h"
#include "pasmparser.h"
#include "adm.h"

int generate_instructions(ParseList *head, Inst program[]);
int main();