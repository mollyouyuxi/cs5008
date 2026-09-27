# include<stdio.h>
# include<stdlib.h>
# include<stdbool.h>

void printBoard(char board[3][3])
{
	printf("Game Board: \n");
	for(int i =0; i<3; i++){
		for (int j =0; j<3; j++){
			printf("%c", board[i][j]);
		}
	printf("\n");
	}
}

// bool isTie(char board[3][3])
// {
// 	return false;
// }

bool isWin(char board[3][3]){
	for (int i=0; i<3; i++){
		// Check Rows
		if(board[i][0] != ' ' && board[i][0] == board[i][1] &&  board[i][1] == board[i][2]){
			return true;
		}
		// Check Columns
		if(board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]){
			return true;
		}
	}

	// Check Diagonally
	if(board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]){
		return true;
	}
	if(board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]){
		return true;
	}
	return false;
}


int main()
{
	printf("Welcome to the TicTacToe game!\n");
	
	// Initialize the game board
	char board[3][3];
	for (int i =0; i<3; i++){
		for (int j=0; j<3; j++){
		board[i][j]=' ';
		}
	}


	char isTurn = 'X';
	int turns = 0;

	while (turns < 9){

		// print out the board
		printBoard(board);

		printf("\nPlayer %c's turn now.\n", isTurn);
		printf("Please enter your move by row and column, seperated by comma: ");

		// get the user input
		char userInput[8];
		fgets(userInput, sizeof(userInput),stdin);
		
		/*
		printf("userInput:%s\n", userInput);
		*/

		// atoi error prone
		// convert the userInput into integer
		char *str_end;
		long userInput_row = strtol(userInput,&str_end, 10);
		if (str_end == userInput) {
    		printf("Invalid input. Row must be a number.\n");
    		continue;
		}
		printf("The row is: %ld\n", userInput_row);

		if (*str_end != ',') {
    		printf("Invalid input. Use the format row,column.\n");
    		continue;
		}

		char *col_start = str_end + 1;
		long userInput_column = strtol(col_start, &str_end, 10);
		if (str_end == col_start) {
			printf("Invalid input. Column must be a number.\n");
			continue;
		}
		printf("The column is: %ld\n", userInput_column);

		if (*str_end != '\n' && *str_end != '\0') {
			printf("Invalid input. Use the format row,column.\n");
			continue;
		}

		// Check the user's move, if invalid, through error message
		// if valid, put the user's move into the board
		if (userInput_row > 3 || userInput_column > 3 || userInput_row <1 || userInput_column<1){
			printf("Invalid move, out of bound. Make sure your input is inside the 3*3 board\n");
			continue;
		}
		if (board[userInput_row-1][userInput_column-1] != ' '){
			printf("This space is already taken, please try again\n");
			continue;
		}

		// Place the move successfully
		board[userInput_row-1][userInput_column-1] = isTurn;
		turns++;

		// Check if one player win the game 
		if (isWin(board)){
			printf("Game End! The winner is %c.\n", isTurn);
			printBoard(board);
			break;
		}
		// Check if tie
		if (turns == 9){
			printf("Game End! Board is full and it's a Tie\n");
			printBoard(board);
			break;
		}

		// Continue the Game
		//Change Player
		if(isTurn == 'X'){
			isTurn = 'O';
		}else{
			isTurn = 'X';
		}

	}

	return 0;

}
