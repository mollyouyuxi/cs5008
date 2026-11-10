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

bool generate_asm(AST_Node *ast, FILE *out_file)
{
    if (ast == NULL)
	{
		printf("ERROR: Root AST node was NULL.\n");
		return false;
	}

	if (ast->kind != AST_PROGRAM)
	{
		printf("ERROR: Root AST node was not PROGRAM. Got kind %s\n", ast_kind_as_cstr(ast->kind));
		return false;
	}
	
	generate_preamble(out_file);
	
	for (AST_Node *fn_node = ast->programs.head; fn_node != NULL; fn_node = fn_node->next)
	{
		bool success = generate_asm_for_fn(fn_node, out_file);
		if (!success) return false;
	}
	
	return true;
}