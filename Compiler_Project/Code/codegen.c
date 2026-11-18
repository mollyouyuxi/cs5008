#include <stdint.h>
void generate_preamble(FILE *out_file)
{
	fprintf(out_file, "global _start\n");
	fprintf(out_file, "\n");
	fprintf(out_file, "_start:\n");
	fprintf(out_file, "    call main\n");
	fprintf(out_file, "    mov rdi, rax\n");
	fprintf(out_file, "    mov rax, 60\n");
	fprintf(out_file, "    syscall\n");
	fprintf(out_file, "\n");
}

bool generate_asm_for_expr(AST_Node *expr_node, FILE *out_file){
    if(expr_node == NULL || expr_node->kind != AST_INTEGER){
        printf("Error: Unsupported expression kind.\n");
        return false;
    }
    long expr = expr_node->int_value;
    fprintf(out_file, "mov rax, %ld\n", expr);
    return true;

}

bool generate_asm_for_fn(AST_Node *fn_node, FILE *out_file)
{
    if(fn_node == NULL || fn_node->kind != AST_FN){
        printf("Error: Node is not a function kind");
        return false;
    }
    String fn_name = fn_node->fn.name;
    fprintf(out_file, "%.*s:\n", Print_String(fn_name));
    AST_Node *stmt = fn_node->fn.body.head;
    while (stmt != NULL){
        if(stmt->kind == AST_RETURN){
            bool expr_gen_result = generate_asm_for_expr(stmt->return_expr, out_file);
            if(expr_gen_result == false) return false;
            fprintf(out_file, "ret\n");
        }else{
            printf("Unsupported statement kind\n");
            return false;
        }  
    
        stmt = stmt->next;
    }
    

	return true;
}



bool generate_asm_for_sm_instr(SM_Instr instr, FILE *out_file)
{
    switch(instr.kind){
        case SM_FN:
            fprintf(out_file, "%.*s:\n", Print_String(instr.fn_name));
            break;
        case SM_END_FN:
            fprintf(out_file, "\n");
            break;
        case SM_PUSH:
            if(instr.int_value >= INT32_MIN && instr.int_value <= INT32_MAX){
                fprintf(out_file, "    push %ld\n", instr.int_value);
            }else{
                // x86-64 push immediates are limited to signed 32-bit values.
                fprintf(out_file, "    mov rax, %ld\n    push rax\n", instr.int_value);
            }
            break;
        case SM_RETURN:
            fprintf(out_file, "    pop rax\n    ret\n");
            break;
        case SM_ADD:
        case SM_SUB:
        case SM_MUL:
        case SM_DIV:
        case SM_MOD:
            // The right operand is on top of the stack.
            fprintf(out_file, "    pop rcx\n    pop rax\n");
            switch(instr.kind){
                case SM_ADD:
                    fprintf(out_file, "    add rax, rcx\n");
                    break;
                case SM_SUB:
                    fprintf(out_file, "    sub rax, rcx\n");
                    break;
                case SM_MUL:
                    fprintf(out_file, "    imul rcx\n");
                    break;
                case SM_DIV:
                case SM_MOD:
                    // Sign-extend rax into rdx:rax before signed division.
                    fprintf(out_file, "    cqo\n    idiv rcx\n");
                    if(instr.kind == SM_MOD){
                        fprintf(out_file, "    mov rax, rdx\n");
                    }
                    break;
                default:
                    break;
            }
            fprintf(out_file, "    push rax\n");
            break;
        default:
            fprintf(stderr, "ERROR: Unsupported stack-machine instruction %d.\n",
                    instr.kind);
            return false;
    }
    return ferror(out_file) == 0;
}

bool generate_asm(SM_Instr_List sm_program, FILE *out_file)
{
    if(out_file == NULL || sm_program.items == NULL || sm_program.count <= 0){
        fprintf(stderr, "ERROR: Expected stack-machine instructions and an output file.\n");
        return false;
    }

    generate_preamble(out_file);

    for(long i = 0; i < sm_program.count; i++){
        if(!generate_asm_for_sm_instr(sm_program.items[i], out_file)){
            return false;
        }
    }
	
	return ferror(out_file) == 0;
}