#include "Header.h"

int main(int argc, char* argv[]) {

    char* dllPathStr = argv[1];
    int iNumA = stoi(argv[2]);
    int iNumB = stoi(argv[3]);

    HMODULE hDLL = LoadLibraryA(dllPathStr);

    if (NULL == hDLL) {
        printf("[DynamicClient] Failed to load DLL.\n");
        return 1;
    }

    MathFunc Add = (MathFunc)GetProcAddress(hDLL, "MathLib_Add");
    MathFunc Subtract = (MathFunc)GetProcAddress(hDLL, "MathLib_Subtract");
    MathFunc Multiply = (MathFunc)GetProcAddress(hDLL, "MathLib_Multiply");
    MathFunc Divide = (MathFunc)GetProcAddress(hDLL, "MathLib_Divide");

    if (NULL == Add) 
    {
        printf("[DynamicClient] Failed to get function addresses.\n");
        FreeLibrary(hDLL);
        return 1;
    }
    if (NULL == Subtract) 
    {
        printf("[DynamicClient] Failed to get function addresses.\n");
        FreeLibrary(hDLL);
        return 1;
    }
    if (NULL == Multiply) 
    {
        printf("[DynamicClient] Failed to get function addresses.\n");
        FreeLibrary(hDLL);
        return 1;
    }
    if (NULL == Divide)
    {
        printf("[DynamicClient] Failed to get function addresses.\n");
        FreeLibrary(hDLL);
        return 1;
    }

    int iAdditionResult = Add(iNumA, iNumB);
    int iSubtractResult = Subtract(iNumA, iNumB);
    int iMultiplyResult = Multiply(iNumA, iNumB);
    int iDividingResult = Divide(iNumA, iNumB);

    printf("Add: %d\n", iAdditionResult);
    printf("Subtract: %d\n", iSubtractResult);
    printf("Multiply: %d\n", iMultiplyResult);
    printf("Dividing: %d\n", iDividingResult);

    FreeLibrary(hDLL);

    return 0;
}
