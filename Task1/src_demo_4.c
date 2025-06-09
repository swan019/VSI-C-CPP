#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define BUFFER_SIZE 1024             // 1KB buffer
#define FILE_SIZE (10 * 1024 * 1024) // 10MB
#define MB (1024 * 1024)             // 1MB

char getRandomChar()
{
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    return letters[rand() % (sizeof(letters) - 1)];
}

char getRandomDigit()
{
    return '0' + (rand() % 10);
}


bool generateFile(char *filename, char *mode)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("fopen failed for '%s' - [%d] %s\n", filename, errno, strerror(errno));
        return false;
    }

    char *buffer = (char *)malloc(BUFFER_SIZE);

    if (!buffer)
    {
        printf("Memory allocation failed for buffer.\n");
        fclose(file);
        file = NULL;
        buffer = NULL;
        return false;
    }

    bool isDigit = strcmp(mode, "digit") == 0;
    if (isDigit)
    {
        for (size_t i = 0; i < BUFFER_SIZE; ++i)
        {
            buffer[i] = getRandomDigit();
        }
    }else{
        for (size_t i = 0; i < BUFFER_SIZE; ++i)
        {
            buffer[i] = getRandomChar();
        }
    }

    size_t count = 0;
    int lastPrintedMB = 0;

    while (count < FILE_SIZE)
    {
        size_t bytesToWrite = (FILE_SIZE - count >= BUFFER_SIZE) ? BUFFER_SIZE : (FILE_SIZE - count);
        size_t written = fwrite(buffer, sizeof(char), bytesToWrite, file);

        if (written != bytesToWrite)
        {
            printf("fwrite failed - '%s' - [%d] %s\n", filename, errno, strerror(errno));
            fclose(file);
            free(buffer);
            file = NULL;
            buffer = NULL;
            return false;
        }

        count += written;
        int currentMB = count / MB;
        if (currentMB > lastPrintedMB)
        {
            printf("Written %dMB...\n", currentMB);
            lastPrintedMB = currentMB;
        }
    }

    printf("Finished writing %.2fMB to file: %s\n", (float)count / MB, filename);
    fclose(file);
    free(buffer);
    file = NULL;
    buffer = NULL;
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

    if (strcmp(mode, "char") != 0 && strcmp(mode, "digit") != 0)
    {
        printf("Invalid mode. Use 'char' or 'digit'.\n");
        return 1;
    }
    bool isFileGenerate = generateFile(filename, mode);
    if (isFileGenerate)
    {
        printf("File creation failed.\n");
        return 1;
    }

    return 0;
}
