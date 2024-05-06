#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 50
#define MAX_COLS 50
#define MAX_PIXEL_LEVELS 5

const char pixel_chars[] = { ' ', '.', 'o', 'O', '0' };

// Function prototypes
void loadImage(char *filename, int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);
void displayImage(const int image[MAX_ROWS][MAX_COLS], int rows, int cols);
void cropImage(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);
void adjustBrightness(int image[MAX_ROWS][MAX_COLS], int rows, int cols, int delta);
void saveImage(char *filename, const int image[MAX_ROWS][MAX_COLS], int rows, int cols);
void editMenu(int image[MAX_ROWS][MAX_COLS], int *rows, int *cols);

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
        printf("0: Exit\n");
        printf("Choose from one of the options above: ");
        scanf("%d", &choice);

        if (choice == 0) break;

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
        printf("Error opening file %s\n", filename);
        return;
    }

    fscanf(file, "%d %d", rows, cols);
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%d", &image[i][j]);
        }
    }

    fclose(file);
    printf("Image successfully loaded!\n");
}

void displayImage(const int image[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", pixel_chars[image[i][j]]);
        }
        printf("\n");
    }
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

    printf("Image cropped!\n");
}

void adjustBrightness(int image[MAX_ROWS][MAX_COLS], int rows, int cols, int delta) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            image[i][j] = (image[i][j] + delta + MAX_PIXEL_LEVELS) % MAX_PIXEL_LEVELS;
        }
    }

    printf("Image brightness adjusted!\n");
}

void saveImage(char *filename, const int image[MAX_ROWS][MAX_COLS], int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for saving\n", filename);
        return;
    }

    fprintf(file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", image[i][j]);
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

    int editChoice;
    printf("Choose an editing option: ");
    scanf("%d", &editChoice);

    if (editChoice == 1) {
        cropImage(image, rows, cols);
    } else if (editChoice == 2) {
        adjustBrightness(image, *rows, *cols, -1); // Dim
    } else if (editChoice == 3) {
        adjustBrightness(image, *rows, *cols, 1); // Brighten
    } else {
        printf("Invalid choice. Returning to main menu.\n");
        return;
    }

    char saveChoice;
    printf("Would you like to save the edited image? (y/n): ");
    scanf(" %c", &saveChoice);

    if (saveChoice == 'y') {
        char filename[100];
        printf("Enter the filename to save the image: ");
        scanf("%s", filename);
        saveImage(filename, image, *rows, *cols);
    }
}
