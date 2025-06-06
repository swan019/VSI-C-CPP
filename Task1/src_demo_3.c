#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "debugger.c"

#define FILE_SIZE (10 * 1024 * 1024) // 10MB
#define BUFFER_SIZE 1024 // 1KB buffer

// Generate a random alphabet character
char getRandomChar()
{
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char charector = letters[rand() % (sizeof(letters) - 1)];
    return charector;
}

// Generate a random digit character
char getRandomDigit()
{
    char digit = '0' + (rand() % 10);
    return digit;
}

bool generateFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Failed to open file.\n");
        return false;
    }

    char buffer[BUFFER_SIZE]; // 1KB buffer
    size_t count = 0;
    int lastPrintedMB = 0;
    bool isDigit = strcmp(mode, "digit") == 0;

    while (count < FILE_SIZE)
    {
        size_t bytesToWrite = (FILE_SIZE - count >= 1024) ? 1024 : (FILE_SIZE - count);

        for (size_t i = 0; i < bytesToWrite; ++i)
        {
            if (isDigit){
                buffer[i] = getRandomDigit();
            } else {
                buffer[i] = getRandomChar();
            }
        }

        size_t written = fwrite(buffer, sizeof(char), bytesToWrite, file);
        if (written != bytesToWrite)
        {
            printf("Write error occurred.\n");
            fclose(file);
            file = NULL;
            return false;
        }

        count += bytesToWrite;

        int currentMB = (int)(count / (1024 * 1024));
        if (currentMB > lastPrintedMB)
        {
            printf("Written %dMB...\n", currentMB);
            lastPrintedMB = currentMB;
        }
    }

    fclose(file);
    file = NULL;
    printf("Finished writing %.2fMB to file: %s\n", (float)count / (1024 * 1024), filename);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <filename> <mode: char | digit>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *mode = argv[2];

    // Validate mode
    if (strcmp(mode, "char") != 0 && strcmp(mode, "digit") != 0)
    {
        printf("Invalid mode. Use 'char' or 'digit'.\n");
        return 1;
    }

    // Generate file
    bool isFileGenerate = generateFile(filename, mode);
    if (!isFileGenerate)
    {
        printf("File creation failed.\n");
        return 1;
    }
    return 0;
}