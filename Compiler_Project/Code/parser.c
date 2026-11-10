typedef enum AST_Kind {
    AST_NONE,

    AST_PROGRAM,
    AST_FN,
    AST_RETURN,
    AST_INTEGER,
    AST_BINOP,
    AST_TYPE
} AST_Kind;

const char *ast_kind_as_cstr(AST_Kind kind)
{
	switch (kind)
	{
	case AST_NONE:    return "NONE (ERROR!)";
	case AST_PROGRAM: return "PROGRAM";
	case AST_FN:      return "FN";
	case AST_TYPE:    return "TYPE";
	case AST_RETURN:  return "RETURN";
	case AST_INTEGER: return "INTEGER";
    case AST_BINOP: return "BINOP";
	default:          return "UNKNOWN (ERROR!)";
	}
}

typedef struct AST_Node AST_Node;

// Holds a sequence of AST Node 
typedef struct AST_List{
    long count;
    AST_Node *head;
    AST_Node *tail;
} AST_List;

typedef struct AST_Fn_Data{
    String name;
    AST_List param;
    Type return_type;
    AST_List body; 
} AST_Fn_Data;

typedef struct AST_Binop_Data{
    AST_Node *lhs;
    AST_Node *rhs;
    Token_Kind operator;
}AST_Binop_Data;

struct AST_Node{
    AST_Node *prev;
    AST_Node *next;
    AST_Kind kind;

    union{
        AST_Fn_Data fn;
        AST_Binop_Data binop;
        AST_List programs;
        long int_value;
        AST_Node *return_expr;
    };
};

typedef struct Parser{
    Token_Array token_arr;
    long token_index;
    bool error_flag;
} Parser;

typedef struct Parse_Result{
    AST_Node *ast;
    bool success;
} Parse_Result;

Token* Parser_Peek(Parser *parser, int offset){
    long peek_index = parser->token_index + offset;
    if (parser->token_arr.count <= peek_index){
        Token *Token_EOF = &parser->token_arr.items[parser->token_arr.count - 1];
        return Token_EOF;
    }
    return &parser->token_arr.items[peek_index];
}


void parser_Error(Parser *parser, Token *actual, const char* msg){
    fprintf(stderr, "%s:%ld:%d: %s", actual->loc.file_name,
            actual->loc.line, actual->loc.col, msg);
    parser->error_flag = true;

}

Token* Parser_Expect_Token(Parser *parser, Token_Kind expect_Kind){
    Token* cur_Token = Parser_Peek(parser, 0);
    if(cur_Token->token_kind != expect_Kind){
        parser_Error(parser, cur_Token, "Unexpected Token.\n");
        printf("Expected token kind: ");
        Print_Token_Kind(expect_Kind);
        printf(", got: ");
        Print_Token_Kind(cur_Token->token_kind);
        printf(".\n");
        return cur_Token;
    }
    parser->token_index++;
    return cur_Token;
}

Token* Parser_Expect_Keyword(Parser *parser, Keyword keyword){
    Token *actual = Parser_Expect_Token(parser, TOKEN_KEYWORD);
    if (parser->error_flag) {
        return actual;
    }
    if(actual->token_data.keyword != keyword){
        parser_Error(parser, actual, "Error: Keyword Unmatched\n");
        fprintf(stderr, "Expected keyword: %s, got: %s\n",
                Keyword_Name(keyword), Keyword_Name(actual->token_data.keyword));
        return actual;
    }
    return actual;
}

AST_Node* Make_AST_Node(AST_Kind kind){
    AST_Node *node = calloc(1, sizeof(AST_Node));
    if (node == NULL){
        fprintf(stderr, "Fail to allocate memory for AST_Node\n");
        exit(1);
    }
    node->kind= kind;
    return node;
}

void AST_List_Append(AST_List *list, AST_Node *node){
    if(list->count == 0){
        list->head = node;
        list->tail = node;
    }else{
        AST_Node *old_tail = list->tail;
        old_tail->next = node;
        node->prev = old_tail;
        list->tail = node;
    }
    list->count++;
}

void Free_AST(AST_Node *node);

void Free_AST_List(AST_List *list){
    AST_Node *node = list->head;
    while (node != NULL){
        // Save the sibling pointer before freeing the current node.
        AST_Node *next = node->next;
        Free_AST(node);
        node = next;
    }
    *list = (AST_List){0};
}

// Frees node and its children
void Free_AST(AST_Node *node){
    if (node == NULL) return;

    switch (node->kind){
        case AST_PROGRAM:
            Free_AST_List(&node->programs);
            break;
        case AST_FN:
            Free_AST_List(&node->fn.param);
            Free_AST_List(&node->fn.body);
            break;
        case AST_RETURN:
            Free_AST(node->return_expr);
            break;
        case AST_BINOP:
            Free_AST(node->binop.lhs);
            Free_AST(node->binop.rhs);
            break;
        case AST_TYPE:
        case AST_INTEGER:
        case AST_NONE:
            break;
    }
    free(node);
}

int get_Precedence(Token_Kind kind){
    switch(kind){
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 1;
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
        case TOKEN_MODULO:
            return 2;
        default:
            return -1;
    }
    
} 

AST_Node *parse_Expr(Parser *parser, int min_prec);

AST_Node* parse_Primary(Parser *parser){
    Token *token = Parser_Peek(parser, 0);
    if(token->token_kind == TOKEN_INTEGER){
        AST_Node *node = Make_AST_Node(AST_INTEGER);
        node->int_value = token->token_data.value;
        parser->token_index++;
        return node;
    }else if(token->token_kind == TOKEN_OPEN_PAREN){
        parser->token_index++;
        AST_Node *node = parse_Expr(parser, 0);
        if (parser->error_flag) return node;
        Parser_Expect_Token(parser, TOKEN_CLOSE_PAREN);
        return node;
    }else{
        parser_Error(parser, token, "Expected a number or '('\n");
        return Make_AST_Node(AST_NONE);
    }
}

AST_Node* parse_Expr(Parser *parser, int min_prec){

    AST_Node *lhs = parse_Primary(parser);
    if(parser->error_flag){
        return lhs;
    }
    // Peek the next token
    Token *operator = Parser_Peek(parser,0);
    int precedence = get_Precedence(operator->token_kind);
    while (precedence > min_prec){
        //Consume
        parser->token_index++;
        AST_Node *rhs = parse_Expr(parser,precedence);
        if(parser->error_flag){
            Free_AST(rhs);
            return lhs;
        }
        // Save the Binop Node
        AST_Node *node = Make_AST_Node(AST_BINOP);
        node->binop.lhs = lhs;
        node->binop.rhs = rhs;
        node->binop.operator = operator->token_kind;
        lhs = node;
        operator = Parser_Peek(parser,0);
        precedence = get_Precedence(operator->token_kind);
    }
    return lhs;

}

AST_Node* parse_Return(Parser *parser){
    Parser_Expect_Keyword(parser, KEYWORD_RETURN);
    if(parser->error_flag){
        return Make_AST_Node(AST_NONE);
    }
    AST_Node *node = Make_AST_Node(AST_RETURN);
    AST_Node *return_expr = parse_Expr(parser,0);
    node->return_expr = return_expr;
    return node;
}

AST_Node* parse_Statement(Parser *parser){
    Token *token = Parser_Peek(parser,0);
    if(token->token_kind != TOKEN_KEYWORD){
        parser_Error(parser, token, "Expected a Keyword kind Token.\n");
        return Make_AST_Node(AST_NONE);
    }
    // Undo: other statements, eg: call_stmt, let, set, etc,.
    if(token->token_data.keyword == KEYWORD_RETURN){
        return parse_Return(parser);
    }else{
        parser_Error(parser, token, "Unsupported Keyword Token.\n");
        return Make_AST_Node(AST_NONE);
    }
}

AST_List parse_Block(Parser *parser){
    AST_List list = {0};
    Parser_Expect_Token(parser, TOKEN_OPEN_BRACE);
    if(parser->error_flag){
        return list;
    }
    Token *token = Parser_Peek(parser,0);
    while(token->token_kind != TOKEN_CLOSE_BRACE 
            && token->token_kind != TOKEN_EOF){
        AST_List_Append(&list, parse_Statement(parser));
        if(parser->error_flag){
            return list;
        }
        token = Parser_Peek(parser,0);
        
    }
    Parser_Expect_Token(parser, TOKEN_CLOSE_BRACE);
    return list;
}

AST_Node* parse_Fn(Parser *parser){
    AST_Node *node = Make_AST_Node(AST_FN);
    Parser_Expect_Keyword(parser, KEYWORD_FN);
    if(parser->error_flag) return node;
    
    Token *token_name = Parser_Expect_Token(parser, TOKEN_IDENT);
    if(parser->error_flag) return node;
    node->fn.name = token_name->source;

    Parser_Expect_Token(parser, TOKEN_OPEN_PAREN);
    if(parser->error_flag) return node;

    // TODO: None-Empty Parameter List

    Parser_Expect_Token(parser, TOKEN_CLOSE_PAREN);
    if(parser->error_flag) return node;

    Token *token_next = Parser_Peek(parser,0);
    if(token_next->token_kind == TOKEN_ARROW){
        parser->token_index++;
        Token *token_rtntype = Parser_Expect_Token(parser, TOKEN_TYPE);
        if(parser->error_flag) return node;
        node->fn.return_type = token_rtntype->token_data.type;
    }
    AST_List fn_body = parse_Block(parser);
    node->fn.body = fn_body;

    return node;
    
}

// The lexer supplies an EOF-terminated token array.
Parse_Result parse_Program(Token_Array tokens){
    Parser parser = {
        .token_arr = tokens,
        .token_index = 0,
        .error_flag = false
    };
    Parse_Result result = {
        .ast = Make_AST_Node(AST_PROGRAM),
        .success = false
    };
    Token *token = Parser_Peek(&parser,0);
    while(token->token_kind != TOKEN_EOF){
        AST_Node *node_fn = parse_Fn(&parser);
        // Preserve partial functions so Free_AST can clean them up.
        AST_List_Append(&result.ast->programs, node_fn);
        if(parser.error_flag) break;
        token = Parser_Peek(&parser,0);
    }
    result.success = !parser.error_flag;
    return result;
}

void print_ast_with_indent(AST_Node *node, int depth){
    if (node == NULL){
        printf("%*s(null)\n", 2 * depth, "");
        return;
    }

    switch (node->kind){
        case AST_PROGRAM:
            printf("%*sprogram\n", 2 * depth, "");
            for (AST_Node *fn = node->programs.head; fn != NULL; fn = fn->next){
                print_ast_with_indent(fn, depth + 1);
            }
            break;
        case AST_FN: {
            printf("%*sfn ", 2 * depth, "");
            if (node->fn.name.data != NULL){
                printf("%.*s", Print_String(node->fn.name));
            }else{
                printf("<missing name>");
            }
            switch (node->fn.return_type){
                case TYPE_NONE: printf("\n"); break;
                case TYPE_INT: printf(" -> int\n"); break;
                case TYPE_STR: printf(" -> str\n"); break;
                case TYPE_BOOL: printf(" -> bool\n"); break;
                default: printf(" -> <unknown type>\n"); break;
            }
            printf("%*sparameters%s\n", 2 * (depth + 1), "",
                   node->fn.param.head == NULL ? ": []" : ":");
            for (AST_Node *param = node->fn.param.head; param != NULL; param = param->next){
                print_ast_with_indent(param, depth + 2);
            }
            printf("%*sbody%s\n", 2 * (depth + 1), "",
                   node->fn.body.head == NULL ? ": []" : ":");
            for (AST_Node *stmt = node->fn.body.head; stmt != NULL; stmt = stmt->next){
                print_ast_with_indent(stmt, depth + 2);
            }
            break;
        }
        case AST_RETURN:
            printf("%*sreturn\n", 2 * depth, "");
            if (node->return_expr != NULL){
                print_ast_with_indent(node->return_expr, depth + 1);
            }
            break;
        case AST_INTEGER:
            printf("%*sinteger %ld\n", 2 * depth, "", node->int_value);
            break;
        case AST_BINOP:
            printf("%*sbinary ", 2 * depth, "");
            Print_Token_Kind(node->binop.operator);
            printf("\n");
            print_ast_with_indent(node->binop.lhs, depth + 1);
            print_ast_with_indent(node->binop.rhs, depth + 1);
            break;
        case AST_NONE:
            printf("%*sAST_NONE (invalid node)\n", 2 * depth, "");
            break;
        default:
            printf("%*sUnknown AST kind: %d\n", 2 * depth, "", node->kind);
            break;
    }
}

void print_ast(AST_Node *node)
{
	print_ast_with_indent(node, 0);
}