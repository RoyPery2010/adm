push 4.0
push 3.0
push 750000

sin:
    swap 1
    swap 1
    ret
loop:
    swap 2

    push 4.0
    dup 2
    push 2.0
    plusf
    swap 3

    divf
    minusf

    push 4.0
    dup 2
    push 2.0
    plusf
    swap 3

    divf
    plusf

    swap 2
    push 1
    minusi

    dup 0

    jmp_if loop

    push 3.14
    call sin

drop
drop

halt
