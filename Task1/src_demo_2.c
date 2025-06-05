#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_SIZE (10 * 1024 * 1024) // 10MB
#define LINE_LENGTH 80

// Generate a random alphabet character
char getRandomChar() {
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char charector = letters[rand() % (sizeof(letters) - 1)];
    return charector;
}

// Generate a random digit character
char getRandomDigit() {
    char digit = '0' + (rand() % 10);
    return digit;
}

// Check if file already exists
bool fileExists(char *filename) {
    FILE *file = fopen(filename, "r");
    if(file != NULL){
        fclose(file);
        return true;
    }else {
        return false;
    }
}

// Create and write to the file
bool generateFile(char *filename, char *mode) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open file.\n");
        return false;
    }

    int count = 0;
    int lastPrintedMB = 0;

    while (count < FILE_SIZE) {
        for (int i = 0; i < LINE_LENGTH - 1 && count < FILE_SIZE; ++i) {
            char ch;
            if (strcmp(mode, "digit") == 0) {
                ch = getRandomDigit(); // For digit
            } else {
                ch = getRandomChar(); // For "character"
            }
            fputc(ch, file);
            count++;
        }
        if (count < FILE_SIZE) {
            fputc('\n', file);
            count++;
        }

        int currentMB = count / (1024 * 1024);
        if (currentMB > lastPrintedMB) {
            printf("Written %dMB...\n", currentMB);
            lastPrintedMB = currentMB;
        }
    }

    fclose(file);
    printf("Finished writing 10MB to file: %s\n", filename);
    return true;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <filename> <mode: char | digit>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *mode = argv[2];

    // Validate mode
    if (strcmp(mode, "char") != 0 && strcmp(mode, "digit") != 0) {
        printf("Invalid mode. Use 'char' or 'digit'.\n");
        return 1;
    }

    // Check if file exists
    bool isFileExists = fileExists(filename);
    if (isFileExists) {
        printf("File '%s' already exists! Overwrite? (y/n): ", filename);
        char choice;
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("Aborting file write.\n");
            return 1;
        }
    }

    // Generate file
    bool isFileGenerate = generateFile(filename, mode);
    if (!isFileGenerate) {
        printf("File creation failed.\n");
        return 1;
    }

    return 0;
}
