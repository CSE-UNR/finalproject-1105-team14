#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 100  
#define MAX_COLS 100  

void read_file_to_2d_array(const char *filename, int array[MAX_ROWS][MAX_COLS], int *rows, int *cols);

int main() {
    int data[MAX_ROWS][MAX_COLS];
    int rows, cols;
	char filename[256];

	printf("Enter file name: ");
	if (scanf("%255s", filename) != 1) {
		printf("Error gettinf file name\n");
		return 1;
	}

    read_file_to_2d_array(filename, data, &rows, &cols);


    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }

    return 0;
}


void read_file_to_2d_array(const char *filename, int array[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
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
            if (ch >= '0' && ch <= '9') {
            	array[row][col] = ch - '0'; 
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
