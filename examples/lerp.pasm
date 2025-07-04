#include "./natives.hpasm"
    jmp main

lerpf:
    dup 3
    dup 3
    dup 1
    plusf
    dup 3
    multf
    plusf

    swap 2
    drop
    swap 2
    drop
    swap 2
    drop
    ret
main:
    push 69.0
    push 420.0
    push 0.0
    push 1.0
    push 10.0
    divf

loop:
    dup 3
    dup 3
    dup 3
    call lerpf
    native print_f64

    swap 1
    dup 1
    plusf
    swap 1

    dup 1
    push 1.0
    gef

    jmp_if loop
    halt
