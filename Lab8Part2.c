#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void printBoard(char board[][26], int n) {
	printf("  ");
	for (int m = 0; m < n; m++) {
		printf("%c", ('a'+m));
	}
	printf("\n"); 
	for (int i = 0; i < n; i++) {
		printf("%c ", ('a'+i));
		for (int j = 0; j < n; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}

}

bool positionInBounds(int n, int row, int col) {
	if (row >= n || col >= n) {
		return false;
	}
	else if (row < 0 || col < 0) {
		return false;
	}
	else {
		return true;
	}
}

bool checkValid(int row, int col, char board[][26], char colour, char otherColour, int Size) {
	int counter = 0;
	// make sure its unfilled
	if(board[row][col] == 'U') {
		//check each row direction
		for(int r = -1; r < 2; r++) {
			//check each column direction
			for(int c = -1; c < 2; c++) {
				//number of opponent pieces in the path
				counter = 0;
				//check that there is an opponent piece adjacent to the position if there is pursue that direction
				while(positionInBounds(Size, row+((counter+1)*r), col+((counter+1)*c)) && board[row+((counter+1)*r)][col+((counter+1)*c)] == otherColour) {
					counter++;
				}
				if (counter != 0 && positionInBounds(Size, row+((counter+1)*r), col+((counter+1)*c)) && board[row+((counter+1)*r)][col+((counter+1)*c)] == colour) {
					return true;
				}
			}
		}
		return false;
	}
	else {
		return false;
	}
}

void deallocate(char***array, int arraySize) {
	for(int m = 0; m < arraySize; m++) {
		free((*array)[m]);
	}
	free(*array);
	*array = NULL;
}
//take old array and fee every part
//make new array and reassign it

void listOfValid(int Size, char colour, char otherColour, const char board[][26], char*** moves, int* added) {
	char **temp = NULL;
	*added = 0;
	
	for(int i = 0; i < Size; i++) {
		for(int j = 0; j < Size; j++) {
			if(checkValid(i, j, board, colour, otherColour, Size)) {
				(*added)++;
				temp = (char **) malloc((*added) * sizeof(char*));
				for(int m = 0; m < (*added); m++) {
					temp[m] = (char *) malloc(3 * sizeof(char));
					if (m < (*added)-1) {
						strcpy(temp[m],(*moves)[m]);
					}
				}
				temp[(*added)-1][0] = i + 'a';
				temp[(*added)-1][1] = j + 'a';
				temp[(*added)-1][2] = '\0';
				if (*moves != NULL) {
					for(int k = 0; k < (*added)-1; k++) {
						free((*moves)[k]);
					}
					free(*moves);
				}
				*moves = temp;
				temp = NULL;	
			}
		}
	}
}

bool checkDirection(char board[][26], int row, int col, char colour, char otherColour, int dir, int Size) {
	int count = 0;
	//going up
	if (dir == 0) {
		while(count+1 < row && board[row-(count+1)][col] == otherColour) {
			count++;
		}
		if(board[row-(count+1)][col] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//going down
	else if (dir == 1) {
		while((count+1) < (Size-1-row) && board[row+count+1][col] == otherColour) {
			count++;
		}
		if(board[row+count+1][col] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//going to the right
	else if (dir == 2) {
		while((count+1) < (Size-1-col) && board[row][col+count+1] == otherColour) {
			count++;
		}
		if(board[row][col+count+1] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//going to the left
	else if (dir == 3) {
		while((count+1) < (col) && board[row][col-(count+1)] == otherColour) {
			count++;
		}
		if(board[row][col-(count+1)] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//diagonal to the upper left
	else if (dir == 4) {
		while((count+1) < (col) && (count+1) < (row) && board[row-(count+1)][col-(count+1)] == otherColour) {
			count++;
		}
		if(board[row-(count+1)][col-(count+1)] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//diagonal to the upper right
	else if (dir == 5) {
		while((count+1) < (Size-1-col) && (count+1) < (row) && board[row-(count+1)][col+(count+1)] == otherColour) {
			count++;
		}
		if(board[row-(count+1)][col+(count+1)] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//diagonal to the lower left
	else if (dir == 6) {
		while((count+1) < (Size-1-row) && (count+1) < (col) && board[row+(count+1)][col-(count+1)] == otherColour) {
			count++;
		}
		if(board[row+(count+1)][col-(count+1)] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
	//diagonal to the lower right
	else if (dir == 7) {
		while((count+1) < (Size-1-row) && (count+1) < (Size-1-col) && board[row+(count+1)][col+(count+1)] == otherColour) {
			count++;
		}
		if(board[row+(count+1)][col+(count+1)] == colour) {
			return true;
		}
		else {
			return false;
		}
	}
}

void flipBoard(char board[][26], int row, int col, char colour, char otherColour, int Size) {
	board[row][col] = colour;
	for (int i = 0; i < 8; i++) {
		if (checkDirection(board, row, col, colour, otherColour, i, Size)) {
			if(i == 0) {
				for(int j = 1; board[row-j][col] == otherColour; j++) {
					board[row-j][col] = colour;
				}
			}
			else if(i == 1) {
				for(int j = 1; board[row+j][col] == otherColour; j++) {
					board[row+j][col] = colour;
				}
			}
			else if(i == 2) {
				for(int j = 1; board[row][col+j] == otherColour; j++) {
					board[row][col+j] = colour;
				}
			}
			else if(i == 3) {
				for(int j = 1; board[row][col-j] == otherColour; j++) {
					board[row][col-j] = colour;
				}
			}
			else if(i == 4) {
				for(int j = 1; board[row-j][col-j] == otherColour; j++) {
					board[row-j][col-j] = colour;
				}
			}
			else if(i == 5) {
				for(int j = 1; board[row-j][col+j] == otherColour; j++) {
					board[row-j][col+j] = colour;
				}
			}
			else if(i == 6) {
				for(int j = 1; board[row+j][col-j] == otherColour; j++) {
					board[row+j][col-j] = colour;
				}
			}
			else if(i == 7) {
				for(int j = 1; board[row+j][col+j] == otherColour; j++) {
					board[row+j][col+j] = colour;
				}
			}
		}
		
	}
}

void makeBoard(char board[][26], int Size) {
    for (int i = 0; i < Size; i++) {
		for(int j = 0; j < Size; j++) {
			if (i == Size/2-1 && j == Size/2-1) {
				board[i][j] = 'W';
			} 
        		else if (i == Size/2-1 && j == Size/2) {
				board[i][j] = 'B';
			}
			else if (i == Size/2 && j == Size/2-1) {
				board[i][j] = 'B';
			}
			else if (i == Size/2 && j == Size/2) {
				board[i][j] = 'W';
			}
        		else {
				board[i][j] = 'U';
			}
		}
	}
}

//function to count how many pieces are in a certain direction
int countScores(char board[][26], int row, int col, char colour, char otherColour, int Size) {
	int numOfPieces = 0;
	for (int i = 0; i < 8; i++) {
		if (checkDirection(board, row, col, colour, otherColour, i, Size)) {
			if(i == 0) {
				for(int j = 1; board[row-j][col] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 1) {
				for(int j = 1; board[row+j][col] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 2) {
				for(int j = 1; board[row][col+j] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 3) {
				for(int j = 1; board[row][col-j] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 4) {
				for(int j = 1; board[row-j][col-j] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 5) {
				for(int j = 1; board[row-j][col+j] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 6) {
				for(int j = 1; board[row+j][col-j] == otherColour; j++) {
					numOfPieces++;
				}
			}
			else if(i == 7) {
				for(int j = 1; board[row+j][col+j] == otherColour; j++) {
					numOfPieces++;
				}
			}
		}
		
	}
	return numOfPieces; 
}

bool checkCorner(char row, char col) {
    if((row == 'h' || row == 'a') && (col == 'h' || col == 'a')) {
        return true;
    }
    else {
        return false;
    }
}

//check number off moves for opponent if piece is placed in that position
int checkScore(const char board[][26], char AI, char enemy, int row, int col, int n) {
    char **oppMoves = NULL;
    int enemyMoves = 0;
    int score = 0;
    char copy[26][26];
    for (int s = 0; s < n; s++) {
        for (int t = 0; t < n; t++) {
            copy[s][t] = board[s][t];
        }
    }
    //insert potential move
    copy[row][col] = AI;
	flipBoard(copy, row, col, AI, enemy, n);
    listOfValid(n, enemy, AI, copy, &oppMoves, &enemyMoves);
    for (int i = 0; i < enemyMoves; i++) {
        if (checkCorner(oppMoves[i][0], oppMoves[i][1])) {
            return 30;
        }
    }
    deallocate(&oppMoves, enemyMoves);
    score = enemyMoves;
    return score;
}

//function for AI to make move, should return a position in form of a string

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
    char** possibleMoves = NULL;
    char enemy = ' ';
    int numOfMoves = 0;
    int score = 0;
    if(turn == 'B') {
        enemy = 'W';
    }
    else {
        enemy = 'B';
    }
    //list of valid positions
    listOfValid(n, turn, enemy, board, &possibleMoves, &numOfMoves);
    if (numOfMoves == 1) {
        *row = possibleMoves[0][0] - 'a';
        *col = possibleMoves[0][1] - 'a';
    }
    else {
        //check if its a diagonal
        for(int a = 0; a < numOfMoves; a++) {
            if(checkCorner(possibleMoves[a][0], possibleMoves[a][1]) == true) {
                (*row) = (possibleMoves[a][0]) - 'a';
                (*col) = (possibleMoves[a][1]) - 'a';
                return 0;
            }
            else if(a == 0){
                score = checkScore(board, turn, enemy, possibleMoves[a][0] - 'a', possibleMoves[a][1] - 'a', n);
                *row = possibleMoves[a][0] - 'a';
                *col = possibleMoves[a][1] - 'a';
            }
            else if(score > checkScore(board, turn, enemy, possibleMoves[a][0] - 'a', possibleMoves[a][1] - 'a', n)) {
                *row = possibleMoves[a][0] - 'a';
                *col = possibleMoves[a][1] - 'a';
            }
        }
    }
    deallocate(&possibleMoves, numOfMoves);
    return 0;
}

//function to get player to input their turn
void getPlayerTurn(int*inputRow, int*inputCol, char playerColour) {
	char row = ' ';
	char col = ' ';
	printf("Enter move for colour %c (RowCol): ", playerColour);
	scanf(" %c%c", &row, &col);
	*inputRow = row - 'a';
	*inputCol = col - 'a';
}

void countPieces(int* emptyPlaces, int* AIPieces, int* playerPieces, char AI, char player, char board[][26], int Size) {
	*emptyPlaces = 0;
	*playerPieces = 0;
	*AIPieces = 0;

	for (int f = 0; f < Size; f++) {
		for (int c = 0; c < Size; c++) {
			if(board[f][c] == AI) {
				(*AIPieces)++;
			}
			else if(board[f][c] == player) {
				(*playerPieces)++;
			}
			else {
				(*emptyPlaces)++;
			}
		}
	}
}

void gamePlay(char AI, char player, int Size, char board[][26]) {
	char gameOver = false;
	bool invalidMove = false;
	int turn = 1;
	int playerRow = 0;
	int playerCol = 0;
	int AIRow = 0;
	int AICol = 0;
	char **playerMoves = NULL;
	char **AIMoves = NULL;
	int numOfAIMoves = 0;
	int numOfPlayerMoves = 0;
	int numOfAI = 0;
	int numOfPlayer = 0;
	int numEmpty = 0;

	while(!gameOver) {
		if(AI == 'B') {
			if(turn % 2 != 0) {
				listOfValid(Size, AI, player, board, &AIMoves, &numOfAIMoves);
				if(numOfAIMoves != 0) {
					makeMove(fdboard, Size, AI, &AIRow, &AICol);
					printf("Computer places %c at %c%c.\n", AI, 'a'+AIRow, 'a'+AICol);
					flipBoard(board, AIRow, AICol, AI, player, Size);
					deallocate(&AIMoves, numOfAIMoves);
				}
				else {
					printf("%c player has no valid move.\n", AI);
				}
				if(numOfAIMoves == 0 && numOfPlayerMoves == 0) {
					gameOver = true;
				}
			}
			else {
				listOfValid(Size, player, AI, board, &playerMoves, &numOfPlayerMoves);
				if(numOfPlayerMoves != 0) {
					getPlayerTurn(&playerRow, &playerCol, player);
					if(checkValid(playerRow, playerCol, board, player, AI, Size)) {
						flipBoard(board, playerRow, playerCol, player, AI, Size);
					}
					else {
						printf("Invalid move.\n");
						invalidMove = true;
					}
				}
				else {
					printf("%c player has no valid move.\n", player);
				}
				if(numOfAIMoves == 0 && numOfPlayerMoves == 0) {
					gameOver = true;
				}
				deallocate(&playerMoves, numOfPlayerMoves);
			}
		}
		else {
			if(turn % 2 != 0) {
				listOfValid(Size, player, AI, board, &playerMoves, &numOfPlayerMoves);
				if(numOfPlayerMoves != 0) {
					getPlayerTurn(&playerRow, &playerCol, player);
					if(checkValid(playerRow, playerCol, board, player, AI, Size)) {
						flipBoard(board, playerRow, playerCol, player, AI, Size);
					}
					else {
						printf("Invalid move.\n");
						invalidMove = true;
					}
				}
				else {
					printf("%c player has no valid move.\n", player);
				}
				if(numOfAIMoves == 0 && numOfPlayerMoves == 0) {
					gameOver = true;
				}
				deallocate(&playerMoves, numOfPlayerMoves);
			}
			else {
				listOfValid(Size, AI, player, board, &AIMoves, &numOfAIMoves);
				if(numOfAIMoves != 0) {
					makeMove(board, Size, AI, &AIRow, &AICol);
					printf("Computer places %c at %c%c.\n", AI, 'a'+AIRow, 'a'+AICol);
					flipBoard(board, AIRow, AICol, AI, player, Size);
				}
				else {
					printf("%c player has no valid move.\n", AI);
				}
				if(numOfAIMoves == 0 && numOfPlayerMoves == 0) {
					gameOver = true;
				}
				deallocate(&AIMoves, numOfAIMoves);
			}
		}
		printBoard(board, Size);
		turn++;
		countPieces(&numEmpty, &numOfAI, &numOfPlayer, AI, player, board, Size);
		if(numEmpty == 0 || gameOver) {
			gameOver = true;
			if(numOfAI > numOfPlayer) {
				printf("%c player wins.\n", AI);
			}
			else if (numOfAI == numOfPlayer) {
				printf("Draw!\n");
			}
			else {
				printf("%c player wins.\n", player);
			}
		}
		else if(invalidMove) {
			printf("%c player wins.\n", AI);
			gameOver = true;
		}
	}
}

int main(void) {
	int Size = 0;
	char AI;
	char player;
	
	printf("Enter the board dimension: ");
	scanf(" %d", &Size);

	while (Size < 0|| Size > 26|| (Size % 2) != 0) {
		printf("Enter the board dimension (must be odd and less than 26): ");
        	scanf(" %d", &Size);
	}
	char board[26][26];
	makeBoard(board, Size);
	printf("Computer plays (B/W): ");
	scanf(" %c", &AI);
	printBoard(board, Size);
    if (AI == 'B') {
        player = 'W';
    }
    else {
        player = 'B';
    }
	gamePlay(AI, player, Size, board);
	return 0;
}
