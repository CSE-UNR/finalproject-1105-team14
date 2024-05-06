//Authors: Dylan Carson & Mohd Shad
//Purpose: Connect 4
//Date: 7 May 2024

#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7
#define SAVE_FILE "saved_game.txt"
#define MAX_NAME_LENGTH 50
#define EMPTY ' '



void mainMenu();
void initializeBoard(char board[ROWS][COLS]);
void displayBoard(const char board[ROWS][COLS]);
int isColumnFull(const char board[ROWS][COLS], int column);
int fullColumn(const char board[ROWS][COLS], int column);
int findEmptyRow(const char board[ROWS][COLS], int column);
void getPlayerInfo(char *player1Name, char *player2Name, char *player1Symbol, char *player2Symbol);
void playerMove(char board[ROWS][COLS], int *currentTurn, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH], char player1Symbol, char player2Symbol, int *exitFlag);
int checkDirection(const char board[ROWS][COLS], int row, int col, int dRow, int dCol, char symbol);
int checkWin(const char board[ROWS][COLS]);
void saveGame(const char board[ROWS][COLS], int currentTurn, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH], char player1Symbol, char player2Symbol);
int loadGame(char board[ROWS][COLS], int *currentTurn, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH], char *player1Symbol, char *player2Symbol);
int checkTie(const char board[ROWS][COLS]);


int main() {
	mainMenu();
	return 0;
}

void mainMenu() {
	char board[ROWS][COLS];
	int currentTurn = 1; 
	int choice;
	char player1Name[MAX_NAME_LENGTH];
	char player2Name[MAX_NAME_LENGTH];
	char player1Symbol;
	char player2Symbol;
	int exitFlag = 0;
	
	printf("Connect 4\n");
	printf("1. Start New Game\n");
	printf("2. Load Saved Game\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);
	
	if (choice == 1) {
		getPlayerInfo(player1Name, player2Name, &player1Symbol, &player2Symbol);
		initializeBoard(board);
	} else if (choice == 2) {
		if (loadGame(board, &currentTurn, player1Name, player2Name, &player1Symbol, &player2Symbol) != 0) {
			printf("No save found, starting a new game.\n");
			getPlayerInfo(player1Name, player2Name, &player1Symbol, &player2Symbol);
			initializeBoard(board);
		}
	}
	else {
		printf("Invalid choice, beginning new game.\n");
		getPlayerInfo(player1Name, player2Name, &player1Symbol, &player2Symbol);
		initializeBoard(board);
	}
	
	displayBoard(board);
	
	while (exitFlag == 0) {
		playerMove(board, &currentTurn, player1Name, player2Name, player1Symbol, player2Symbol, &exitFlag);
		
		if (exitFlag == 1) {
			break;
		}
		
		displayBoard(board);
		if (checkWin(board)) {
			printf("%s wins!\n", currentTurn == 1 ? player2Name : player1Name);
			saveGame(board, currentTurn, player1Name, player2Name, player1Symbol, player2Symbol);
			mainMenu();
			break;
		}
		
		if (checkTie(board)) {
			printf("It's a tie! No more possible moves.\n");
			mainMenu();
			saveGame(board, currentTurn, player1Name, player2Name, player1Symbol, player2Symbol);
			break;
		}
		saveGame(board, currentTurn, player1Name, player2Name, player1Symbol, player2Symbol);
	}
}


void getPlayerInfo(char *player1Name, char *player2Name, char *player1Symbol, char *player2Symbol) {
	printf("Enter Player 1's Name: ");
	scanf("%s", player1Name);
	printf("Enter Player 1's Symbol: ");
	scanf(" %c", player1Symbol);
	
	printf("Enter Player 2's Name: ");
	scanf("%s", player2Name);
	printf("Enter Player 2's Symbol: ");
	scanf(" %c", player2Symbol);
	
	
	while (*player1Symbol == *player2Symbol) {
		printf("Player 1 and 2 Must use different symbols, enter a new symbol for player 2: ");
		scanf(" %c", player2Symbol);
	}
}
		
	
	


void initializeBoard(char board[ROWS][COLS]) {
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			board[i][j] = EMPTY;
		}
	}
}

void displayBoard(const char board[ROWS][COLS]) {
	printf("\n");
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			printf("| %c ", board[i][j]);
		}
		printf("\n");
	}
	for (int j = 1; j <= COLS; ++j) {
		printf("  %d ", j);
	}
	printf("\n\n");
}

int isColumnFull(const char board[ROWS][COLS], int column) {
	return board[0][column] != EMPTY;
}

int findEmptyRow(const char board[ROWS][COLS], int column) {
	for (int i = ROWS - 1; i >=0; --i) {
		if(board[i][column] == EMPTY) {
			return i;
			}
		}
	return -1;
}

void playerMove(char board[ROWS][COLS], int *currentTurn,  char player1Name[MAX_NAME_LENGTH],  char player2Name[MAX_NAME_LENGTH], char player1Symbol, char player2Symbol, int *exitFlag) {
	int column;
	char symbol = (*currentTurn == 1) ? player1Symbol : player2Symbol;
	const char *playerName = (*currentTurn == 1) ? player1Name: player2Name;
	
	do {
		printf("At any time enter 0 to Exit\n%s (%c), enter your column (1-%d): ", playerName, symbol, COLS);
		scanf("%d", &column);
		
		
		
		column -= 1;
		
		if (column == -1) {
			printf("Exiting the game. Your game is saved!\n");
			*exitFlag = 1;
			saveGame(board, *currentTurn, player1Name, player2Name, player1Symbol, player2Symbol);
			return;
		}
			 
	} while (column < 0 || column >= COLS || isColumnFull(board, column));
	
	int row = findEmptyRow(board, column);
	board[row][column] = symbol;
	
	*currentTurn = (*currentTurn == 1) ? 2: 1;
}

int checkDirection(const char board[ROWS][COLS], int row, int col, int dRow, int dCol, char symbol) {
	int count = 0;
	for( int i = 0; i < 4; ++i) {
		int r = row + i * dRow;
		int c = col + i * dCol;
		if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != symbol) {
			return 0;
		}
		count++;
	}
	return count == 4;
}

int checkWin(const char board[ROWS][COLS]) {
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			char symbol = board[i][j];
			if (symbol == EMPTY) {
				continue;
			}
			
			if (checkDirection(board, i, j, 0, 1, symbol) ||
				checkDirection(board, i, j, 1, 0, symbol) ||
				checkDirection(board, i, j, 1, 1, symbol) ||
				checkDirection(board, i, j, 1, -1, symbol))
				{
					return 1;
				}
			}
		}
		return 0;
}

void saveGame(const char board[ROWS][COLS], int currentTurn,  char player1Name[MAX_NAME_LENGTH],  char player2Name[MAX_NAME_LENGTH], char player1Symbol, char player2Symbol) {
	FILE *file = fopen(SAVE_FILE, "w");
	if (!file) {
		printf("Error saving the game.\n");
		return;
	}
	
	fprintf(file, "%s\n%s\n%c\n%c\n", player1Name, player2Name, player1Symbol, player2Symbol);
	
	
	fprintf(file, "%d\n", currentTurn);
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			fputc(board[i][j], file);
		}
		fputc('\n', file);
	}
	
	fclose(file);
	printf("Game saved successfully.\n\n");
}

int loadGame(char board[ROWS][COLS], int *currentTurn,  char player1Name[MAX_NAME_LENGTH],  char player2Name[MAX_NAME_LENGTH], char *player1Symbol, char *player2Symbol) {
	FILE *file = fopen(SAVE_FILE, "r");
	if (!file) {
		return -1;
	}

	
	fscanf(file, "%s\n%s\n %c\n %c\n", player1Name, player2Name, player1Symbol, player2Symbol);


	fscanf(file, "%d\n", currentTurn);
	

	
		for (int i = 0; i < ROWS ; ++i) {
			for (int j = 0; j < COLS; ++j){
				char c = fgetc(file);
				if (c != *player1Symbol && c != *player2Symbol && c != '\n') {
					board[i][j] = EMPTY;
				}
				else if (c == *player1Symbol || c == *player2Symbol){ 
					board[i][j] = c;
				}
			}
			fgetc(file);
		}
	fclose(file);
	
	if (checkWin(board)) {
		displayBoard(board);
		printf("%s won the game that was loaded!\n\n", *currentTurn == 1 ? player2Name : player1Name);
		mainMenu();
		return 0;
	} else if (checkTie(board)) {
		displayBoard(board);
		printf("The game that was loaded resulted in a tie!\n\n");
		mainMenu();
		
		return 0;
	} else {
	}

	return 0;
}	
	
	
int checkTie(const char board[ROWS][COLS]) {
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			if (board[i][j] == EMPTY) {
				return 0;
			}
		}
	}
	return 1;
}
