#include <stdio.h>  //header file

int main(int argc, char *argv[]){
	/*
	printf("argc = %d\n", argc);
	for (int i=0; i < argc; i++ ){
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	char msg[] = "Molly";
	*/

	/*
	printf("Hello, %s!\n", argv[1]);
	return 0;
	*/
	for (int i=0; i <argc; i++){
		printf("Hello, %s\n", argv[i]);
	}
	
}
