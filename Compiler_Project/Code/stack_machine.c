typedef enum SM_Kind{
    SM_FN,
    SM_END_FN,
    SM_RETURN,
    SM_PUSH,
    SM_ADD,
    SM_SUB,
    SM_MUL,
    SM_DIV,
    SM_MOD
}SM_Kind;

typedef struct SM_Instr{
    SM_Kind kind;

    union{
    String fn_name;
    long int_value;
    };

}SM_Instr;

typedef struct SM_Instr_List{
    SM_Instr *items;
    long count;
    long capacity;
}SM_Instr_List;

void SM_Instr_Append(SM_Instr_List *list, SM_Instr sm_instr){
    if (list->capacity <= list->count){
        if(list->capacity == 0){
            list->capacity = 16;
        } else{
            list->capacity *= 2;
        }    

        SM_Instr *new_items = realloc(list->items, list->capacity*sizeof(sm_instr));

        if(new_items == NULL){
            fprintf(stderr, "Error: Failed to allocate memory for instruction.\n");
            exit(1);
        }
        
    list->items = new_items;
    }

    list->items[list->count] = sm_instr;
    list->count++; 
}

void Print_SM_Instr(SM_Instr sm_instr){
    switch(sm_instr.kind){
        case SM_FN:
            printf("FN %.*s\n", Print_String(sm_instr.fn_name));
            break;
        case SM_END_FN:
            printf("END_FN\n");
            break;
        case SM_RETURN:
            printf("RETURN\n");
            break;
        case SM_PUSH:
            printf("PUSH %ld\n", sm_instr.int_value);
            break;
        case SM_ADD:
            printf("ADD\n");
            break;
        case SM_SUB:
            printf("SUB\n");
            break;
        case SM_MUL:
            printf("MUL\n");
            break;
        case SM_DIV:
            printf("DIV\n");
            break;
        case SM_MOD:
            printf("MOD\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown stack-machine instruction kind %d.\n",
                    sm_instr.kind);
            break;
    }
}

void Print_SM_Instr_List(SM_Instr_List *sm_list){
    for(long i = 0; i < sm_list->count; i++){
        Print_SM_Instr(sm_list->items[i]);
    }
}


// Constructors create instruction records; they do not append or execute them.
SM_Instr Make_SM_Push(long int_value){
    SM_Instr result = {
        .kind = SM_PUSH,
        .int_value = int_value,
    };
    return result;
}

// The name borrows its storage, just like the function name in the AST.
SM_Instr Make_SM_Fn(String fn_name){
    SM_Instr result = {
        .kind = SM_FN,
        .fn_name = fn_name,
    };
    return result;
}

SM_Instr Make_SM_End_Fn(void){
    SM_Instr result = {
        .kind = SM_END_FN,
    };
    return result;
}

SM_Instr Make_SM_Return(void){
    SM_Instr result = {
        .kind = SM_RETURN,
    };
    return result;
}

SM_Instr Make_SM_Add(void){
    SM_Instr result = {
        .kind = SM_ADD,
    }; 
    return result;
}

SM_Instr Make_SM_Sub(void){
    SM_Instr result = {
        .kind = SM_SUB,
    };
    return result;
}

SM_Instr Make_SM_Mul(void){
    SM_Instr result = {
        .kind = SM_MUL,
    };
    return result;
}

SM_Instr Make_SM_Div(void){
    SM_Instr result = {
        .kind = SM_DIV,
    };
    return result;
}

SM_Instr Make_SM_Mod(void){
    SM_Instr result = {
        .kind = SM_MOD,
    };
    return result;
}

void Generate_SM_Expr(SM_Instr_List *list, AST_Node *expr){
    
    if(expr == NULL){
        fprintf(stderr, "Error: Expected a non-null expression.\n");
        exit(1);
    }
    // For an integer node, append the value in the instruction
    if(expr->kind == AST_INTEGER){
        SM_Instr_Append(list, Make_SM_Push(expr->int_value));
        return;
    }

    if(expr->kind != AST_BINOP){
        fprintf(stderr, "Error: Unsupported expression kind %d.\n", expr->kind);
        exit(1);
    }

    // If not integer node, generate the lhs and rhs subtree
    Generate_SM_Expr(list, expr->binop.lhs);

    Generate_SM_Expr(list, expr->binop.rhs);

    // Generate the operator
    SM_Instr instr;
    switch(expr->binop.operator){
        case TOKEN_PLUS: 
            instr = Make_SM_Add(); 
            break;
        case TOKEN_MINUS: 
            instr = Make_SM_Sub(); 
            break;
        case TOKEN_MULTIPLY: 
            instr = Make_SM_Mul(); 
            break;
        case TOKEN_DIVIDE: 
            instr = Make_SM_Div(); 
            break;
        case TOKEN_MODULO:  
            instr = Make_SM_Mod(); 
            break;
        default:
            fprintf(stderr, "Error: Unsupported binary operator %d.\n",
                    expr->binop.operator);
            exit(1);
    }

    SM_Instr_Append(list, instr);
}

SM_Instr_List generate_Stack_Machine_Instr(AST_Node *ast){
    SM_Instr_List result = {
        .items = NULL,
        .count = 0,
        .capacity = 0,
    };

    if(ast == NULL || ast->kind != AST_PROGRAM){
        fprintf(stderr, "Error: Expected a non-null AST_PROGRAM root.\n");
        exit(1);
    }
    
    AST_Node *program = ast;

    AST_Node *fn = program->program.head;
    while(fn != NULL){
        if(fn->kind != AST_FN){
            printf("Expecting AST_FN kind node\n");
            exit(1);
        }
        
        SM_Instr_Append(&result, Make_SM_Fn(fn->fn.name));

        // Generate the SM instruction for function body
        AST_Node *stmt = fn->fn.body.head;
        while(stmt != NULL){
            if(stmt->kind != AST_RETURN){
                fprintf(stderr, "Error: Expected an AST_RETURN statement.\n");
                exit(1);
            }

            Generate_SM_Expr(&result, stmt->return_expr);
            SM_Instr_Append(&result, Make_SM_Return());
            stmt = stmt->next;
        }

        SM_Instr_Append(&result, Make_SM_End_Fn());
        fn = fn->next;
    }


    return result;
}
