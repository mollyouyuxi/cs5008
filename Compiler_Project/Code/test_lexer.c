#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.c"

int main(int arg_count, const char **args){
    if (arg_count < 2){
        printf("Usage: %s <source_file.jive>\n", args[0]);
        return 1;
    }

    const char *file_name = args[1];
    
    String source = readFile(file_name);
    Token_Array token_arr = Tokenize(source, file_name);
	if (token_arr.count == 0) {
        printf("Empty token array\n");
		return 1;
	}
	
	bool test_lexer = true;
	if (test_lexer)
	{
		printf("Lexer output:\n");
		Print_Token_Array(&token_arr);
    }
    
    free(token_arr.items);
    free(source.data);
    return 0;
}