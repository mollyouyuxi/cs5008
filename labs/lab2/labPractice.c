#include <stdio.h>


int main(){
    /*

    int a, b, c;
    a = 123;
    b = 234;
    c = 345;
    int *pa, *pb, *pc;
    pa = &a;
    pb = &b;
    pc = &c;

    // Print the value address directly
    printf("a = %d at address: %p\n", a, &a);
    printf("b = %d at address: %p\n", b, &b);
    printf("c = %d at address: %p\n", c, &c);

    // Print the pointer address
    printf("a = %d at address: %p\n", a, pa);
    printf("b = %d at address: %p\n", b, pb);
    printf("c = %d at address: %p\n", c, pc);

    // Dereferencing and modify the value
    *pa = 100;
    printf("Change the value via pointer");
    printf("The value of 'a' now change to %d\n", a);
    */


    /*

    int arr[4] = {0, 1, 2, 3};
    // The arr is a pointer points to the adress of the first element
    int *p = arr;

    // Cast p pointer to pint to char type and examine the performance
    char *charptr = (char *)p;
    // get the address of the pointer
    printf("The address of the pointer is %p\n", &p);

    printf("The array is at address %p\n", arr);
    for (int i = 0; i < 4; i++){
        printf("arr[%d] = %d is at the address: %p\n", i, arr[i], &arr[i]);
        printf("The pointer is %p\n", p);
        printf("The char_pointer is %p\n", charptr);
        p++;
        charptr++;
    }
    */

    int a = 1234; // binary representation: 0000....010011010010(32bit)  -> 00/00/04/d2(hex, 4byte) -> d20400 (little endian)
    printf("Address: %p, Value : %d, Hexidecimal: %x \n", &a, a, a);
    char *char_pointer = (char *) &a;
    for (int i = 0; i <4; i++){
        printf("Address: %p, Value: %d, Hexidecimal: %x\n", char_pointer, (unsigned char)*char_pointer, (unsigned char)*char_pointer);
        char_pointer++;
    }
    return 0;
}