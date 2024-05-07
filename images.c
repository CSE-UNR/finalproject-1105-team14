//Authors: Dylan Carson & Mohd Shad
//Purpose: Image Editing & Processing
//Date: 7 May 2024


#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000
#define MAPPINGSIZE 5

void loadImage(char *filename, int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);
void displayImage(const int image[MAX_ROWS][MAX_COLS], int rows, int cols);
void cropImage(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);
void adjustBrightness(int image[MAX_ROWS][MAX_COLS], int rows, int cols, int delta);
void saveImage(char *filename, const int image[MAX_ROWS][MAX_COLS], int rows, int cols);
void editMenu(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);
void rotateCounter(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);

int main() {
    int image[MAX_ROWS][MAX_COLS] = {0};
    int rows = 0, cols = 0;
    int choice;
    char filename[100];


    while (true) {
        printf("\n**Instagram**\n");

        printf("1: Load image\n");
        printf("2: Display image\n");
        printf("3: Edit image\n");
        printf("0: Exit\n\n");
        printf("Choose from one of the options above: ");
        scanf("%d", &choice);

        if (choice == 0) {
        	printf("\n\nGoodbye!\n\n"); 
        	break;
		}
        switch (choice) {
            case 1:
                printf("What is the name of the image file? ");
                scanf("%s", filename);
                loadImage(filename, image, &rows, &cols);
                break;
            case 2:
                displayImage(image, rows, cols);
                break;
            case 3:
                editMenu(image, &rows, &cols);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

void loadImage(char *filename, int image[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char ch;
    *rows = 0;
    *cols = 0;
    int row = 0;
    int col = 0;
    bool first_row = true;
    int temp_cols = 0;


    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (first_row) {
                *cols = temp_cols;
                first_row = false;
            }
            (*rows)++;
            temp_cols = 0;
            row++;
            col=0;
        } else {
            if (ch >= '0' && ch <= '4') {
            	image[row][col] = ch - '0'; 
            	col++;
            	temp_cols++;
            }
        }
    }


    if (col > 0) {
        if (first_row) {
            *cols = col;
        }
        (*rows)++;
    }

	fclose(file);
}


void displayImage(const int image[MAX_ROWS][MAX_COLS], int rows, int cols) {
	const char mapping[] = {' ', '.', 'o', 'O', '0'};
	const int mappingSize = sizeof(mapping) / sizeof(mapping[0]);

	
	printf("\n");
	for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int value = image[i][j];
            if (value >= 0 && value < mappingSize) {
            printf("%c", mapping[value]);
        } else {
        	printf("?");
        }
    }
    printf("\n");
}
printf("\n\n");
}


void cropImage(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
    int left, right, top, bottom;

    printf("What would you like the first column on the left to be: ");
    scanf("%d", &left);
    printf("What would you like the last column on the right to be: ");
    scanf("%d", &right);
    printf("What would you like the top row to be: ");
    scanf("%d", &top);
    printf("What would you like the bottom row to be: ");
    scanf("%d", &bottom);

    int new_rows = bottom - top + 1;
    int new_cols = right - left + 1;
    int cropped[MAX_ROWS][MAX_COLS] = {0};

    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < new_cols; j++) {
            cropped[i][j] = image[top + i][left + j];
        }
    }

    *rows = new_rows;
    *cols = new_cols;

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            image[i][j] = cropped[i][j];
        }
    }

    printf("\nImage cropped!\n");
}

void adjustBrightness(int image[MAX_ROWS][MAX_COLS], int rows, int cols, int delta) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int new_brightness = image[i][j] + delta ;
		if (new_brightness < 0) {
			new_brightness = 0;
		} else if (new_brightness >= MAPPINGSIZE) {
			new_brightness = MAPPINGSIZE - 1;
		}
		
		image[i][j] = new_brightness;
		
        }
    }

    printf("\nImage brightness adjusted!\n");
}

void rotateCounter(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
	int rotated[MAX_COLS][MAX_ROWS] = {0};
	int newRows = *cols;
	int newCols = *rows;
	
	for (int i = 0; i < *rows; ++i) {
		for (int j = 0; j < *cols; ++j) {
			rotated[*cols - j - 1][i] = image[i][j];
		}
	}
	
	*rows = newRows;
	*cols = newCols;

	for (int i = 0; i < *rows; ++i) {
		for (int j = 0; j < *cols; ++j) {
			image[i][j] = rotated[i][j];
		}
	}

	printf("\nImage Rotated Counterclockwise!\n");
}

void saveImage(char *filename, const int image[MAX_ROWS][MAX_COLS], int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for saving\n", filename);
        return;
    }


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d", image[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Image saved to %s!\n", filename);
}

void editMenu(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
    printf("\n**Instagram - Edit Menu**\n");


    printf("1: Crop image\n");
    printf("2: Dim image\n");
    printf("3: Brighten image\n");
    printf("4: Rotate Counterclockwise\n\n");

    int editChoice;
    printf("Choose an editing option: ");
    scanf("%d", &editChoice);

    if (editChoice == 1) {
        cropImage(image, rows, cols);
    } else if (editChoice == 2) {
        adjustBrightness(image, *rows, *cols, -1); // Dim
    } else if (editChoice == 3) {
        adjustBrightness(image, *rows, *cols, 1); // Brighten
    } else if (editChoice == 4) {
    	rotateCounter(image, rows, cols);
    } 
    else {
        printf("Invalid choice. Returning to main menu.\n");
        return;
    }

	displayImage(image, *rows, *cols);
	
    char saveChoice;
    printf("\nWould you like to save the edited image? (y/n): ");
    scanf(" %c", &saveChoice);

    if (saveChoice == 'y') {
        char filename[100];
        printf("\nEnter the filename to save the image: ");
        scanf("%s", filename);
        saveImage(filename, image, *rows, *cols);
    }
}
