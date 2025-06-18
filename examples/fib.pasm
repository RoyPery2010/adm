#include "./natives.hpasm"

main:
    push 0.0
    push 1.0
    push 30
loop:
    swap 2
    dup 0
    native 4
    dup 1
    plusi
    swap 1
    swap 2
    push 1
    minusi

    dup 0
    push 0
    eq

    not

    jmp_if loop
    halt
