;SyscallNumber    arg0     arg1    arg2     arg3
;   rax          rdi     rsi     rdx      rlo
;    1(write)     fd    adress  length

;    fdr (file descriptors): stdin  0
;                            stdout 1
;                             stderr 2



global _start

section .data
    newline db 10

section .text

_start:
   ; push 98

    ; To print 1A2B
    mov r8, 10
    add r8, 'A' -10
    push r8

    mov rax, 1  ; write
    mov rdi, 1  ; file descriptors
    mov rsi, rsp
    mov rdx, 1  ; length should be 1 byte since little endian of 70 is 0x46
    syscall     ; The ouput we generate should be "F" as ASCII

    ; print a new line
    ; mov byte[rsp], 10 ;ASCII for "/n" is 10
    mov rsi, newline
    mov rax, 1
    mov rdi, 1
    mov rdx, 1 
    syscall

    mov rax, 60
    mov rdi, 20 
    syscall
