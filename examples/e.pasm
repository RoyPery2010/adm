    push 1.0
    push 1.0
    push 1.0
loop:
    push 1.0
    dup 2
    divf
    plusf

    swap 2
    push 1.0
    plusf
    dup 0
    swap 2
    multf

    swap 1
    swap 2

    dup 2
    push 100.0
    gef

    jmp_if loop
    halt
