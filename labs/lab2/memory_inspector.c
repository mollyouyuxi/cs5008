#include <stdio.h>

struct Course {
    int id;
    const char *name;
};

void inspect_memory(char *address, long byte_count){
    // 16 bytes each line
    for (long line = 0; line < byte_count; line +=16){

        printf("%p  ", (address + line));

        // print the hex representation 
        for (long i = 0; i < 16; i++){
            if(line + i < byte_count){
                printf("%02x ",(unsigned char)address[line+i]);
            }else{
                // else if there are less than 16 bytes remained
                printf("   ");
            }
        }
        printf("  ");

        // print the ASCII
        for (long i = 0; i < 16; i++){

            // check remaining bytes
            if(line + i < byte_count){
                unsigned char char_ascii = (unsigned char)address[line+i];
                if(char_ascii < 32 || char_ascii > 126){
                    printf(".");
                }else{
                    printf("%c", char_ascii);
                }
            }

        }
        printf("\n");
    }


}

int main(){
    int a = 10086;
    char c = 42;

    struct Course course = {
        .id = 5008,
        .name = "Data Struct, Algorithm & Application in CmpSys"
    };

    int arr[4] = {2, 56, 24, 19};
    const char *str = "Hi, this is Molly. Nice to meet you!";


    // Inspect the memory of an int
    printf("Inpect the memory of an int\n");
    inspect_memory((char *)&a, sizeof(a));

    // Inspect the memory of a char
    printf("Inpect the memory of a char\n");
    inspect_memory((char *)&c, sizeof(c));

    // Inspect the memory of a struct
    printf("Inpect the memory of a struct\n");
    inspect_memory((char *)&course, sizeof(course));

    // Inspect the memory of an array
    printf("Inpect the memory of an array\n");
    inspect_memory((char *)arr, sizeof(arr));

    // Inspect the memory of a string
    printf("Inpect the memory of a string\n");
    inspect_memory((char *)str, sizeof("Hi, this is Molly. Nice to meet you!"));

    return 0;
}