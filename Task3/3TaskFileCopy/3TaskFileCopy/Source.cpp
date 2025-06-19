#include <stdio.h>
#include <windows.h>
#include <stdbool.h>  

bool fileCopy(char* srcPath, char* destPath) {
    BOOL copyFile = CopyFileA(srcPath, destPath, FALSE);

    if (copyFile != 0) {
        printf("File copied successfully.\n");
        return true;
    }
    else {
        DWORD errorCode = GetLastError();
        printf("Failed to copy file. Error code: %lu\n", errorCode);
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Src File Path> <Dest File Path>\n", argv[0]);
        return 1;
    }

    char* srcPath = argv[1];
    char* destPath = argv[2];

    bool isFileCopy = fileCopy(srcPath, destPath);

    if (!isFileCopy) {
        return 1; 
    }

    return 0; 
}
