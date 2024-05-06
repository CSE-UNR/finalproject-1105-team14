#include <stdio.h>
#include <stdbool.h>

#define MAX_IMAGE_SIZE 100
const char *grayScale = " .o00̷";
int numLevels = 5;

void loadNewImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols);
void displayImage(const char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols);
void editImageMenu(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols);
void cropImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols);
void dimImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols);
void brightenImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols);
void rotateImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols);
void saveImage(const char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols);

int main() {
    char image[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];
    int imageRows = 0, imageCols = 0;
    int choice;

    while (true) {
        printf("\nImage Processing Program\n");
        printf("1. Load a New Image\n");
        printf("2. Display the Current Image\n");
        printf("3. Edit the Current Image\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loadNewImage(image, &imageRows, &imageCols);
                break;
            case 2:
                displayImage(image, imageRows, imageCols);
                break;
            case 3:
                editImageMenu(image, imageRows, imageCols);
                break;
            case 4:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void loadNewImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols) {
    char filename[100];
    FILE *file;
    printf("Enter the name of the image file to load: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open the file '%s'.\n", filename);
        return;
    }

    fscanf(file, "%d%d", rows, cols);
    for (int i = 0; i < *rows; ++i) {
        for (int j = 0; j < *cols; ++j) {
            int pixelValue;
            fscanf(file, "%d", &pixelValue);
            if (pixelValue >= numLevels) {
                pixelValue = numLevels - 1;
            }
            img[i][j] = grayScale[pixelValue];
        }
    }

    fclose(file);
    printf("Image loaded successfully.\n");
}

void displayImage(const char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols) {
    if (rows == 0 || cols == 0) {
        printf("No image to display.\n");
        return;
    }
    printf("\nDisplaying the current image:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%c", img[i][j]);
        }
        printf("\n");
    }
}

void editImageMenu(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols) {
    int choice;
    while (true) {
        printf("\nEdit Menu\n");
        printf("1. Crop\n");
        printf("2. Dim\n");
        printf("3. Brighten\n");
        printf("4. Rotate 90 Degrees\n");
        printf("5. Save and Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                cropImage(img, &rows, &cols);
                break;
            case 2:
                dimImage(img, rows, cols);
                break;
            case 3:
                brightenImage(img, rows, cols);
                break;
            case 4:
                rotateImage(img, &rows, &cols);
                break;
            case 5:
                saveImage(img, rows, cols);
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }

        displayImage(img, rows, cols);
    }
}

void cropImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols) {
    int newLeft, newRight, newTop, newBottom;

    printf("Which column do you want to be the new left side? ");
    scanf("%d", &newLeft);
    printf("Which column do you want to be the new right side? ");
    scanf("%d", &newRight);
    printf("Which row do you want to be the new top? ");
    scanf("%d", &newTop);
    printf("Which row do you want to be the new bottom? ");
    scanf("%d", &newBottom);

    if (newLeft < 0 || newRight >= *cols || newTop < 0 || newBottom >= *rows || newLeft > newRight || newTop > newBottom) {
        printf("Invalid crop range.\n");
        return;
    }

    int newRows = newBottom - newTop + 1;
    int newCols = newRight - newLeft + 1;

    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            img[i][j] = img[newTop + i][newLeft + j];
        }
    }

    *rows = newRows;
    *cols = newCols;
    printf("Image cropped successfully.\n");
}

void dimImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int level = -1;
            for (int k = 0; k < numLevels; ++k) {
                if (img[i][j] == grayScale[k]) {
                    level = k;
                    break;
                }
            }
            if (level > 0) level--;
            img[i][j] = grayScale[level];
        }
    }
    printf("Image dimmed successfully.\n");
}

void brightenImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int level = -1;
            for (int k = 0; k < numLevels; ++k) {
                if (img[i][j] == grayScale[k]) {
                    level = k;
                    break;
                }
            }
            if (level < numLevels - 1) level++;
            img[i][j] = grayScale[level];
        }
    }
    printf("Image brightened successfully.\n");
}

void rotateImage(char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int *rows, int *cols) {
    char temp[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];

    for (int i = 0; i < *cols; ++i) {
        for (int j = 0; j < *rows; ++j) {
            temp[i][j] = img[*rows - 1 - j][i];
        }
    }

    int tempDim = *rows;
    *rows = *cols;
    *cols = tempDim;

    for (int i = 0; i < *rows; ++i) {
        for (int j = 0; j < *cols; ++j) {
            img[i][j] = temp[i][j];
        }
    }

    printf("Image rotated successfully.\n");
}

void saveImage(const char img[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE], int rows, int cols) {
    char filename[100];
    printf("Enter the name of the file to save the edited image: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Unable to open the file '%s' for saving.\n", filename);
        return;
    }

    fprintf(file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int level = -1;
            for (int k = 0; k < numLevels; ++k) {
                if (img[i][j] == grayScale[k]) {
                    level = k;
                    break;
                }
            }
            fprintf(file, "%d ", level);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Image saved successfully to '%s'.\n", filename);
}
