global _start

_start:
    call main
    mov rdi, rax
    mov rax, 60
    syscall

main:
mov rax, 42
ret
