#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

bool generateFile(char* fileName) {
    FILE* pFile;
    pFile = fopen(fileName, "wb");
    if (!pFile) {
        fprintf(stderr, "Error: Failed to open file - [%d] %s\n",
            errno, strerror(errno));
        return false;
    }

    char* chBuffer = (char*)malloc(sizeof(char) * ONE_KB);
    if (chBuffer == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory - [%d] %s\n",
            errno, strerror(errno));
        fclose(pFile);
        return false;
    }

    for (int i = 0; i < TEN; i++) {
        char ch = 'a' + i;
        memset(chBuffer, ch, ONE_KB);

        for (int j = 0; j < ONE_MB / ONE_KB; ++j) {
            size_t written = fwrite(chBuffer, sizeof(char), ONE_KB, pFile);
            if (written != ONE_KB) {
                fprintf(stderr, "Error: Failed to write to file at %dMB block [%d] %s\n",
                    i + 1, errno, strerror(errno));
                free(chBuffer);
                fclose(pFile);
                return false;
            }
        }
    }

    free(chBuffer);
    fclose(pFile);
    chBuffer = NULL;
    pFile = NULL;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <FileName>\n", argv[0]);
        return 1;
    }

    char* fileName = argv[1];
    bool isFileGenerate = generateFile(fileName);
 
    if (isFileGenerate == false) {
        printf("File created failed <%s>.\n", argv[0]);
        return 1;
    }
    return 0;
}



