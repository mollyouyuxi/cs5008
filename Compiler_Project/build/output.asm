global _start

_start:
    call main
    mov rdi, rax
    mov rax, 60
    syscall

main:
    push 42
    pop rax
    ret

