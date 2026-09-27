#include <stdio.h>
#include <stdlib.h>

void swapByValue(int a, int b){
    int temp = a;
    a = b;
    b = temp;
}

void swapByPointer(int *a, int *b){
    int * temp = a;
    a = b; 
    b = temp;
}

int main(){
    int a = 10;
    int b = 20;
    int c = 10;
    int d = 20;
    printf("Before swapping, a is %d, b is %d\n", a, b);
    swapByValue(a, b);
    printf("After swapping, a is %d, b is %d\n", a, b);
    printf("Swapping by pointer\n");
    printf("Before swapping, a is %d, b is %d\n", c, d);
    swapByPointer(&c, &d);
    printf("After swapping, a is %d, b is %d\n", c, d);
    return 0;
}