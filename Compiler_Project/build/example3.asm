global _start

_start:
    call main
    mov rdi, rax
    mov rax, 60
    syscall

main:
    push 2
    push 3
    push 4
    pop rcx
    pop rax
    imul rcx
    push rax
    pop rcx
    pop rax
    add rax, rcx
    push rax
    push 5
    pop rcx
    pop rax
    sub rax, rcx
    push rax
    pop rax
    ret

