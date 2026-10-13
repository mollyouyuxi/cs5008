global _start

section .text

_start:
	mov rax, 0     ; The number to print
	
	mov rcx, rsp        ; rcx will store the beginning of the output string
	sub rsp, 16         ; Reserve 16 bytes on the stack for the output string
	
.while:
	test rax, rax
	jz .end_while       ; while rax != 0
	
	mov rdx, rax
	and rdx, 0xF        ; rdx = last four bits of rax
	shr rax, 4          ; Chop off last four bits from rax
	
	cmp rdx, 10
	jge .else           ; if rdx < 10
	add rdx, '0'        ;     rdx += '0' (turn it into an ascii digit)
	jmp .endif
.else:                  ; else
	add rdx, 'a' - 10   ;     rdx += 'a' - 10 (turn it into an ascii a-f)
.endif:
	
	dec rcx				; We will write one character, so move rcx back to accommodate
	mov [rcx], dl       ; Write the character to the stack at rcx (dl is the last byte of rdx)
	jmp .while
.end_while:
	
	mov rax, 1          ; write syscall
	mov rdi, 1          ; file_descriptor = 1 = stdout
	mov rsi, rcx        ; buffer = rcx
	neg rcx                   ; Calculate count = (rsp + 16 - rcx)
	lea rdx, [rsp + rcx + 16] ; Use lea to do it in fewer instructions
	syscall
	
	mov rax, 1          ; write syscall
	mov rdi, 1          ; file_descriptor = 1 = stdout
	mov rsi, rsp        ; buffer = rsp (overwrite previous buffer)
	mov byte [rsi], 10  ; buffer[0] = '\n' (new line = ascii code 10)
	mov rdx, 1          ; count = 1
	syscall
	
	add rsp, 16         ; Restore stack pointer (NOTE: not necessary since we exit the process rather than return from a function)
	
	mov rax, 60         ; exit syscall
	mov rdi, 0          ; return_code = 0 (success!)
	syscall
