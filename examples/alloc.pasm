#include "./natives.hpasm"
    jmp main
main:
    push 420
    native alloc
    dup 0
    native print_ptr
    native free
    halt
