#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct String{
    long length;
    char *data;
} String;

#define Counted_String(str) (String){sizeof(str-1), str}

#define Print_String(str) (int) (str).length, (str).data

bool String_Equals(String a, String b){
    if (a.length != b.length){
        return false;
    }
    for (long i = 0, j = 0; i < a.length && j < b.length; i++, j++){
        if(a.data[i] != b.data[j]){
            return false;
        }
    }
    return true;
}

bool String_Equals_ConstChar(String a, const char* b){
    long b_length = (long) strlen(b);
    if (a.length != b_length){
        return false;
    }
    for (long i = 0, j = 0; i < a.length && j < b_length; i++, j++){
        if(a.data[i] != b[j]){
            return false;
        }
    }
    return true;
}