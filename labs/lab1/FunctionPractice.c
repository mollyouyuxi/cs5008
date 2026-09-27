#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// int add(int a, int b){
//     return a + b;
// }

// int subtract(int a, int b){
//     return a - b;
// }

// int multiple(int a, int b){
//     return a * b;
// }

// int devide(int a, int b){
//     return a / b;
// }

// int modules(int a, int b){
//     return a % b;
// }

// int main(){
//     int a = 10;
//     int b = 6;
//     printf("Addition: %d\n", add(a,b));
//     printf("Subtract: %d\n", subtract(a, b));
//     printf("Multiple: %d\n", multiple(a,b));
//     printf("Devide: %d\n", devide(a, b));
//     printf("Remainder: %d\n", modules(a, b));
//     return 0;
// }

// void simpleCondition(int lower, int upper){
//     if(lower < upper){
//         printf("lower is less than upper\n");
//     } else if(lower > upper){
//         printf("lower is greater than upper\n");
//     } else{
//         printf("lower is equal to upper\n");
//     }
// }

// int main(){
//     int lower = 5;
//     int upper =10;
//     printf("Simple condition practice with lower: %d, upper: %d\n", lower, upper);
//     simpleCondition(lower,upper);
//     return 0;
// }

void forLoopPractice(int start, int end){
    for (int i = start; i < end; i++){
        printf("For loop %d\n", i);

    }
}

void reverseLoop(int start, int end){
    for (int i = end-1; i >= start; i--){
         printf("Reverse loop %d\n", i);
    }
}

void jumpLoop(int start, int end){
    for (int i = start; i < end; i += 2){
        printf("Jump loop %d\n", i);
    }
}

void whileLoop(int start, int end){
    while(start < end){
        printf("While loop %d\n", start);
        start++;
    }
}

void whileloopMenu(){
    int choice = 0;
    while (choice != 3){
        printf("1. For loop\n");
        printf("2. While loop\n");
        printf("3. Exit\n");
        scanf("%d", &choice);
        
        switch(choice){
            case 1:
                forLoopPractice(0,10);
                break;
            case 2:
                whileLoop(0,10);
                break;
            case 3:
                printf("Exiting\n");
                break;
            default:
                printf("Invalid choice. Try again\n");

        }        
    }

}

int main(){
    // forLoopPractice(0, 10);
    // reverseLoop(0, 10);
    // jumpLoop(0, 10);
    // whileLoop(0, 10);
    whileloopMenu();
    return 0;
}