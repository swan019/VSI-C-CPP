#include "Header.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 

bool FileGenerate(char* fileName)
{
    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "CreateFile failed (%lu)\n", GetLastError());
        return false;
    }

    char* chBuffer = (char*)malloc(ONE_KB);
    if (chBuffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        CloseHandle(hFile);
        return false;
    }

    bool success = true;
    for (int i = 0; i < TEN; i++)
    {
        char ch = 'a' + i;
        memset(chBuffer, ch, ONE_KB);

        for (int j = 0; j < ONE_MB / ONE_KB; j++)
        {
            DWORD bytesWritten;
            BOOL isByteWriteInWriteFile = WriteFile(hFile, chBuffer, ONE_KB, &bytesWritten, NULL);
            if (!isByteWriteInWriteFile || bytesWritten != ONE_KB)
            {
                fprintf(stderr, "WriteFile failed at %dMB block (%lu)\n", i + 1, GetLastError());
                success = false;
                break;
            }
        }
        if (!success) break;
    }

    free(chBuffer);
    CloseHandle(hFile);
    return success;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <FileName>\n", argv[0]);
        return 1;
    }

    char fullPath[MAX_PATH];
    if (!PathCombineA(fullPath, BASE_DIR, argv[1]))
    {
        fprintf(stderr, "Failed to construct full file path.\n");
        return 1;
    }

    bool IsFileGenerated = FileGenerate(fullPath);
    if (!IsFileGenerated)
    {
        fprintf(stderr, "File creation failed.\n");
        return 1;
    }

    printf("File successfully created and written.\n");
    return 0;
}