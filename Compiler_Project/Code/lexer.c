#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "string.c"

typedef enum Token_Kind{
    TOKEN_KEYWORD,
    TOKEN_IDENT,
    TOKEN_TYPE,
    TOKEN_INTEGER,
    TOKEN_STRING,
    TOKEN_ARROW,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_OPEN_BRACKET,   
    TOKEN_CLOSE_BRACKET, 
    // Arithmetic
    TOKEN_PLUS,           // +
    TOKEN_MINUS,          // -
    TOKEN_MULTIPLY,       // *
    TOKEN_DIVIDE,         // /
    TOKEN_MODULO,         // %
    // Bitwise
    TOKEN_BITWISE_AND,    // &
    TOKEN_BITWISE_OR,     // |
    TOKEN_BITWISE_XOR,    // ^
    TOKEN_BITWISE_NOT,    // ~
    // Logical
    TOKEN_LOGICAL_AND,    // &&
    TOKEN_LOGICAL_OR,     // ||
    TOKEN_LOGICAL_NOT,    // !
    // Comparison
    TOKEN_EQUAL,          // ==
    TOKEN_NOT_EQUAL,      // !=
    TOKEN_LESS,           // <
    TOKEN_GREATER,        // >
    TOKEN_LESS_EQUAL,     // <=
    TOKEN_GREATER_EQUAL,  // >=
    TOKEN_COMMA,          // ,
    TOKEN_DOT,            // .
    TOKEN_COLON,          // :
    TOKEN_EOF
} Token_Kind;

typedef enum Keyword{
    KEYWORD_FN,
    KEYWORD_LET,
    KEYWORD_SET,
    KEYWORD_IF,
    KEYWORD_WHILE,
    KEYWORD_CALL,
    KEYWORD_RETURN,
    KEYWORD_TRUE,
    KEYWORD_FALSE
} Keyword;

typedef enum Type{
    TYPE_INT,
    TYPE_STR,
    TYPE_BOOL
} Type;


//Location of the Token
typedef struct Loc{
    const char *file_name;
    long line;
    int col;
} Loc;

typedef struct Token{
    Token_Kind token_kind;
    String source; //Original text
    Loc loc;

    // specific token info
    union{
        Keyword keyword;
        Type type;
        long value;
    } token_data;
}Token;

typedef struct Token_Array{
    long count;
    long capacity;
    Token *items;
} Token_Array;

typedef struct Lexer
{
	String source;
	long index;
	Loc loc;
} Lexer;
 
char Lexer_Peek(Lexer *lexer){
    if(lexer->index >= lexer-> source.length){
        return '\0';
    }
    return lexer->source.data[lexer->index];
}

char Lexer_Advance(Lexer *lexer){
    char cur = Lexer_Peek(lexer);

    if (lexer->index >= lexer-> source.length){
        return '\0';
    }

    lexer->index ++;

    if(cur == '\n'){
        lexer->loc.col = 1;
        lexer->loc.line++;
    }else{
        lexer->loc.col++;
    }
    return cur;
}

void Lexer_Skip_WhiteSpace(Lexer *lexer){
    char cur = Lexer_Peek(lexer);
    while (lexer->index < lexer->source.length &&
            isspace((unsigned char) cur)){

        Lexer_Advance(lexer);
        cur = Lexer_Peek(lexer);
    }
}

void Lexer_Skip_Comment(Lexer *lexer){
    char cur = Lexer_Peek(lexer);
    if(cur == '/' &&
        lexer->index+1 < lexer->source.length
        && lexer->source.data[lexer->index+1] == '/'){
        while(lexer->index < lexer->source.length &&
                cur != '\n'){
            Lexer_Advance(lexer);
            cur = Lexer_Peek(lexer);
        }
    }
}

Token Lexer_Identifier_Keyword(Lexer *lexer){
    Loc start_loc = lexer->loc;
    long start = lexer->index;
    char cur = Lexer_Peek(lexer);
    while (lexer->index < lexer->source.length &&
        (isalnum((unsigned char)cur) || cur == '_') 
        ){
        Lexer_Advance(lexer);
        cur = Lexer_Peek(lexer);
    }
    long length = lexer->index - start;
    String str = {
        .length = length,
        .data = lexer->source.data+start, 
    };

    Token token = {0};

    if(String_Equals_ConstChar(str, "fn")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_FN;
    }else if(String_Equals_ConstChar(str, "let")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_LET;
    }else if(String_Equals_ConstChar(str, "set")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_SET;
    }else if(String_Equals_ConstChar(str, "if")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_IF;
    }else if(String_Equals_ConstChar(str, "while")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_WHILE;
    }else if(String_Equals_ConstChar(str, "call")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_CALL;
    }else if(String_Equals_ConstChar(str, "return")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_RETURN;
    }else if(String_Equals_ConstChar(str, "true")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_TRUE;
    }else if(String_Equals_ConstChar(str, "false")){
        token.token_kind = TOKEN_KEYWORD;
        token.token_data.keyword = KEYWORD_FALSE;
    }else if(String_Equals_ConstChar(str,"int")){
        token.token_kind = TOKEN_TYPE;
        token.token_data.type = TYPE_INT;
    }else if(String_Equals_ConstChar(str,"str")){
        token.token_kind = TOKEN_TYPE;
        token.token_data.type = TYPE_STR;
    }else if(String_Equals_ConstChar(str,"bool")){
        token.token_kind = TOKEN_TYPE;
        token.token_data.type = TYPE_BOOL;
    }else{
        token.token_kind = TOKEN_IDENT;

    }
    token.source = str;
    token.loc= start_loc;
    return token;
}

Token Lexer_Num(Lexer *lexer){
    Loc start_loc = lexer->loc;
    long start = lexer->index;
    char cur = Lexer_Peek(lexer);
    while(lexer->index < lexer->source.length && isdigit((unsigned char)cur)){
        Lexer_Advance(lexer);
        cur = Lexer_Peek(lexer);
    }
    long length = lexer->index - start;
    String str = {
        .length = length,
        .data = lexer->source.data+start, 
    };

    Token token ={
        .token_kind = TOKEN_INTEGER,
        .source = str,
        .loc = start_loc,
        .token_data.value = strtol(str.data, NULL, 10)
    };
    return token;
}

Token Lexer_Symbol(Lexer *lexer){
    Loc start_loc = lexer->loc;
    long start = lexer->index;
    char cur = Lexer_Peek(lexer);
    Token token = {0};
    switch(cur){
        case '+':
            token.token_kind = TOKEN_PLUS;
            Lexer_Advance(lexer);
            break;

        case '-':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '>') {
                token.token_kind = TOKEN_ARROW;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_MINUS;
            }
            break;

        case '*':
            token.token_kind = TOKEN_MULTIPLY;
            Lexer_Advance(lexer);
            break;

        case '/':
            token.token_kind = TOKEN_DIVIDE;
            Lexer_Advance(lexer);
            break;

        case '%':
            token.token_kind = TOKEN_MODULO;
            Lexer_Advance(lexer);
            break;

        case '&':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '&') {
                token.token_kind = TOKEN_LOGICAL_AND;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_BITWISE_AND;
            }
            break;

        case '|':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '|') {
                token.token_kind = TOKEN_LOGICAL_OR;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_BITWISE_OR;
            }
            break;

        case '^':
            token.token_kind = TOKEN_BITWISE_XOR;
            Lexer_Advance(lexer);
            break;

        case '~':
            token.token_kind = TOKEN_BITWISE_NOT;
            Lexer_Advance(lexer);
            break;

        case '!':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '=') {
                token.token_kind = TOKEN_NOT_EQUAL;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_LOGICAL_NOT;
            }
            break;

        case '=':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '=') {
                token.token_kind = TOKEN_EQUAL;
                Lexer_Advance(lexer);
            } else {
                fprintf(stderr, "Error: Unexpected '='\n");
                exit(1);
            }
            break;

        case '<':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '=') {
                token.token_kind = TOKEN_LESS_EQUAL;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_LESS;
            }
            break;

        case '>':
            Lexer_Advance(lexer);
            if (Lexer_Peek(lexer) == '=') {
                token.token_kind = TOKEN_GREATER_EQUAL;
                Lexer_Advance(lexer);
            } else {
                token.token_kind = TOKEN_GREATER;
            }
            break;

        case '(':
            token.token_kind = TOKEN_OPEN_PAREN;
            Lexer_Advance(lexer);
            break;

        case ')':
            token.token_kind = TOKEN_CLOSE_PAREN;
            Lexer_Advance(lexer);
            break;

        case '{':
            token.token_kind = TOKEN_OPEN_BRACE;
            Lexer_Advance(lexer);
            break;

        case '}':
            token.token_kind = TOKEN_CLOSE_BRACE;
            Lexer_Advance(lexer);
            break;

        case '[':
            token.token_kind = TOKEN_OPEN_BRACKET;
            Lexer_Advance(lexer);
            break;

        case ']':
            token.token_kind = TOKEN_CLOSE_BRACKET;
            Lexer_Advance(lexer);
            break;

        case ',':
            token.token_kind = TOKEN_COMMA;
            Lexer_Advance(lexer);
            break;

        case '.':
            token.token_kind = TOKEN_DOT;
            Lexer_Advance(lexer);
            break;

        case ':':
            token.token_kind = TOKEN_COLON;
            Lexer_Advance(lexer);
            break;

        default:
            fprintf(stderr, "Error: Unknown symbol '%c'\n", cur);
            exit(1);
    }
    
    long length = lexer->index - start;
    String str = {
        .length = length,
        .data = lexer->source.data + start
    };   
    token.source = str;
    token.loc = start_loc;
    return token;
}

Token Lexer_String(Lexer *lexer){
    Loc start_loc = lexer->loc;
    long start = lexer->index;

    String str = {0};

    Lexer_Advance(lexer);
    char cur = Lexer_Peek(lexer);

    while (cur != '"'){
        if(cur == '\0' || cur == '\n'){
            fprintf(stderr, "Invalid String\n");
            exit(1);
        }
        str.length++;
        Lexer_Advance(lexer);
        cur = Lexer_Peek(lexer);
    }
    
    str.data = lexer->source.data+start+1;
    Token token = {
        .token_kind = TOKEN_STRING,
        .loc = start_loc,
        .source = str
    };
    Lexer_Advance(lexer);
    return token;
}

void Token_Append(Token_Array *arr, Token token){
    // Check if current array have enough space 
    if (arr->capacity <= arr->count){
        if(arr->capacity == 0){
            arr->capacity = 16;
        } else{
            arr->capacity *= 2;
        }    
        Token *new_items = realloc(arr->items, arr->capacity*sizeof(Token));
        
        if(new_items == NULL){
            fprintf(stderr, "Error: Failed to allocate memory for tokens\n");
            exit(1);
        }  
        arr->items = new_items;
    } 
    arr->items[(arr->count)]=token;
    arr->count++;
}


void Print_Token(Token token){
    const char *name = strrchr(token.loc.file_name, '/');
    int printed = printf("%s:%ld:%d ",name+1, token.loc.line, token.loc.col);
    int padding = 24 - printed;
    if (padding < 1) {
        padding = 1;
    }

    printf("%*s", padding, "");

    switch(token.token_kind){
        case TOKEN_KEYWORD:
            printf("%-15s %.*s\n", "KEYWORD", Print_String(token.source));
            break;
        case TOKEN_IDENT:
            printf("%-15s %.*s\n", "IDENTIFIER", Print_String(token.source));
            break;
        case TOKEN_TYPE:
            printf("%-15s %.*s\n", "TYPE", Print_String(token.source));
            break;
        case TOKEN_INTEGER:
            printf("%-15s %.*s\n", "INTEGER", Print_String(token.source));
            break;
        case TOKEN_STRING:
            printf("%-15s %.*s\n", "STRING", Print_String(token.source));
            break;
        case TOKEN_ARROW:
            printf("->\n");
            break;
        case TOKEN_OPEN_PAREN:
            printf("(\n");
            break;
        case TOKEN_CLOSE_PAREN:
            printf(")\n");
            break;
        case TOKEN_OPEN_BRACE:
            printf("{\n");
            break;
        case TOKEN_CLOSE_BRACE:
            printf("}\n");
            break;
        case TOKEN_OPEN_BRACKET:
            printf("[\n");
            break;
        case TOKEN_CLOSE_BRACKET:
            printf("]\n");
            break;
        case TOKEN_PLUS:
            printf("+\n");
            break;
        case TOKEN_MINUS:
            printf("-\n");
            break;
        case TOKEN_MULTIPLY:
            printf("*\n");
            break;
        case TOKEN_DIVIDE:
            printf("/\n");
            break;
        case TOKEN_MODULO:
            printf("%%\n");
            break;
        case TOKEN_BITWISE_AND:
            printf("&\n");
            break;
        case TOKEN_BITWISE_OR:
            printf("|\n");
            break;
        case TOKEN_BITWISE_XOR:
            printf("^\n");
            break;
        case TOKEN_BITWISE_NOT:
            printf("~\n");
            break;
        case TOKEN_LOGICAL_AND:
            printf("&&\n");
            break;
        case TOKEN_LOGICAL_OR:
            printf("||\n");
            break;
        case TOKEN_LOGICAL_NOT:
            printf("!\n");
            break;
        case TOKEN_EQUAL:
            printf("==\n");
            break;
        case TOKEN_NOT_EQUAL:
            printf("!=\n");
            break;
        case TOKEN_LESS:
            printf("<\n");
            break;
        case TOKEN_GREATER:
            printf(">\n");
            break;
        case TOKEN_LESS_EQUAL:
            printf("<=\n");
            break;
        case TOKEN_GREATER_EQUAL:
            printf(">=\n");
            break;
        case TOKEN_COMMA:
            printf(",\n");
            break;
        case TOKEN_DOT:
            printf(".\n");
            break;
        case TOKEN_COLON:
            printf(":\n");
            break;
        case TOKEN_EOF:
            printf("EOF\n");
            break;
        default:
            printf("OTHER\n");
            break;
    }
}

void Print_Token_Array(Token_Array *arr){
    if(arr->items == NULL){
        fprintf(stderr, "Error: Invalid Array.\n");
        exit(1);
    }
    for (int i = 0; i < arr->count; i++){
        Print_Token(arr->items[i]);
    }
}

Token_Array Tokenize(String source, const char* file_name){
    Token_Array token_arr = {0};

    Loc loc = {
        .file_name = file_name,
        .line = 1,
        .col = 1
    };

    Lexer lexer = {
        .source = source,
        .index = 0,
        .loc = loc
    };

    char cur = Lexer_Peek(&lexer);

    while(lexer.index < lexer.source.length){

        if(cur == '_' || isalpha((unsigned char)cur)){                                  // Identifier or Keyword
            Token_Append(&token_arr, Lexer_Identifier_Keyword(&lexer));
        }else if(isdigit((unsigned char)cur)){                                          // Integer
            Token_Append(&token_arr, Lexer_Num(&lexer));
        }else if(cur == '"'){                                                           // String   
            Token_Append(&token_arr, Lexer_String(&lexer));
        }else if(isspace((unsigned char)cur)){                                          // White Space
            Lexer_Skip_WhiteSpace(&lexer);
        }else if(cur == '/' && 
                lexer.index+1 < lexer.source.length && 
                lexer.source.data[lexer.index+1] == '/'){                           //Comment
            Lexer_Skip_Comment(&lexer);
        }else{
            Token_Append(&token_arr, Lexer_Symbol(&lexer));
        }

        cur = Lexer_Peek(&lexer);
    }
    
    // End of the file, append the EOF Token 
    Token eof = {
        .token_kind = TOKEN_EOF,
        .source = {
            .length = 0,
            .data = NULL
        },
        .loc = {
            .file_name = file_name,
            .line = lexer.loc.line,
            .col = lexer.loc.col
        },
    };
    Token_Append(&token_arr, eof);
    return token_arr;
}    

String readFile(const char* file_name){
    // Read the file and store it into memory
    FILE *file = fopen(file_name, "r");
    if (file == NULL){
        fprintf(stderr, "Error: Invalid File.\n");
        exit(1);
    }

    // go to the end and find out the size
    int seek_result = fseek(file, 0, SEEK_END);
    if (seek_result != 0) {
        fprintf(stderr, "Error: Could not seek to the end of file.\n");
        fclose(file);
        exit(1);
    }

    long size = ftell(file);
    if(size<0){
        fprintf(stderr, "Error: Fail to retrieve the file size\n");
        exit(1);
    }

    // return to beginning of the file
    rewind(file);

    char* buffer = malloc((size_t)size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file buffer\n");
        fclose(file);
        exit(1);
    }

    size_t bytesRead = fread(buffer, 1, (size_t)size, file);
    if (bytesRead != (size_t)size) {
        free(buffer);
        fclose(file);
        exit(1);
    }

    buffer[bytesRead] = '\0';
    fclose(file);

    String result = {
        .length = (long) bytesRead,
        .data = buffer
    };

    return result;
}
