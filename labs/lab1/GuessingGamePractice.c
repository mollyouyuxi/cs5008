#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int getRandomNumber (int min, int max){
    // max - min + 1 : how many numbers are there btw min & max;
    // rand % max - min + 1 : force the random number to be less than (max - min + 1)
    // (rand() % (max - min +1)) + min : adding the min make sure we are at the min-max range now 
    return (rand() % (max - min +1)) + min;
}

int getInput(){
    // Declare a location of input where we are going to store it;
    int input = 0;
    // printf("Please enter a number\n");
    scanf("%d", &input);
    return input;
}

int runGame(int randomNumber){
    int guess = 0;
    int attempts = 0;

    while(guess != randomNumber){
        printf("Enter your guess between 1 to 100: \n");
        guess = getInput();
        attempts++;

        if(guess < randomNumber){
            printf("Your guess is too low\n");
        }else if(guess > randomNumber){
            printf("Your guess is too high\n");
        }else{
            printf("Congratulations! You guessed the number %d in %d attempts. \n", randomNumber, attempts);
        }
    }
    return attempts;
}

void start(){
    printf("Welcome to the guessing game\n");
    int choice = 0;
    while (choice != 2){
        printf("1. Start Game\n");
        printf("2. Eixt\n");
        printf("Enter your choice:");
        choice = getInput();

        if(choice == 1){
            int randomNumber = getRandomNumber(1, 100);
            runGame(randomNumber);
        }else if(choice == 2){
            printf("Exiting\n");
            break;
        }else {
            printf("Invalid input, please try again\n");
        }
    }
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    start();
    return 0;
}