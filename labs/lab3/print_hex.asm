global _start

section .data
    newline db 10
    zero db 48
    

section .text

_start:

    ;mov rax, 0x1A2B ; goal to print (Binary 0001 1010 0010 1011, Decimal 6699)
    mov rax, 0xffffffffffffff00 
    ;mov rax, 0


    ; check if the rax is negative or zero
    cmp rax, 0
    jl .negResultConvert
    je .printZero
    mov r8, 0 ; if positive, r8 is 0 
    jmp .signChecked
        .negResultConvert:
        mov r8, 1 ; store the negative sign

.signChecked:

	mov rcx, rsp        ; store the address of the beginning of the output string
    ;sub rsp, 16         ; Reserve 16 bytes on the stack for the output string
	sub rsp, 16         ; Reserve 16 bytes on the stack for the output string

    .while:
        test rax, rax ;test if rax is 0 and we print all the content in rax 

        jz .end_while ; end the loop the zero flag

        ; if not reach the end, continue print rax 
        ;mov rdx, rax 
        ;and rdx, 0xF ; read four bits from rax and store it in rdx
        ;shr rax, 4 ; Shrift right and chop off last four bits from rax

        ; if not reach the end, continue print rax 
        mov rdi, 10 ;intantiate the devisor 
        cqo ; sgn extend
        idiv rdi ; (rax is 0x1A2B, and rdx is 0x0000, after divided by 10, rax is 0x1A0, rdx is 9, ect)

        ; check if rdx is negative 
        cmp rdx, 0
        jl .negativeRemainder
        jmp .negativeChecked
        
        .negativeRemainder:
            neg rdx
    .negativeChecked:

        ; convert the rdx content
        ; if its a digit, rdx < 10
        ; if its a letter, rdx >= 10
        cmp rdx, 10
        jge .else
        add rdx, 48
        jmp .endif
    .else:
        add rdx, 55
    .endif:
        sub rcx, 1 ;move the buffer(address) by 1 byte
        mov [rcx], dl ; store the low 8 bits of RDX (DL) into memory at [RCX]
        ; continue read the next hex
        jmp .while
    .end_while:


    cmp r8, 1
    je .negativeResult
    jmp .printResult
        .negativeResult:
        sub rcx, 1
        mov byte [rcx], '-'

.printResult:
    ; syscall to write 
    mov rax, 1
    mov rdi, 1 ;fd 1, stdout
    mov rsi, rcx ;buffer
    neg rcx
    lea rdx, [rsp+16+rcx]; count, the actual memory used is [rps-rcx+16]
    syscall
    jmp .printNewLine

.printZero:
    ; syscall to write 
    mov rax, 1
    mov rdi, 1 ;fd 1, stdout
    mov rsi, zero ;buffer
    mov rdx, 1; count
    syscall

.printNewLine:
; Print the new line
mov rax, 1
mov rdi, 1
mov rsi, newline
mov rdx, 1
syscall

; Exit with code 0
mov rax, 60
mov rdi, 0
syscall