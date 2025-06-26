#include <windows.h>
#include <iostream>
#include <string>

typedef int(*MATHFUNC)(int, int);

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: <exe> <DLL Path> <num1> <num2>\n");
        return -1;
    }

    char* dllPathStr = argv[1];

    int iNum1 = std::stoi(argv[2]);
    int iNum2 = std::stoi(argv[3]);

    HMODULE hModule = LoadLibraryA(dllPathStr);
    if (hModule == NULL) 
    {
        fprintf(stderr, "LoadLibrary failed. Error\n");
        return -1;
    }

    MATHFUNC Add = (MATHFUNC)GetProcAddress(hModule, "Add");
    MATHFUNC Subtract = (MATHFUNC)GetProcAddress(hModule, "Subtract");
    MATHFUNC Multiply = (MATHFUNC)GetProcAddress(hModule, "Multiply");

    if (NULL == Add) 
    {
        fprintf(stderr, "GetProcAddress of Add failed. Error: (%lu)\n", GetLastError());
        FreeLibrary(hModule);
        return -1;
    }

    if (NULL == Subtract) 
    {
        fprintf(stderr, "GetProcAddress of Subtract failed. Error: (%lu)\n", GetLastError());
        FreeLibrary(hModule);
        return -1;
    }

    if (NULL == Multiply) 
    {
        fprintf(stderr, "GetProcAddress of Multiply failed. Error: (%lu)\n", GetLastError());
        FreeLibrary(hModule);
        return -1;
    }

    printf("Add(%d, %d) = %d\n", iNum1, iNum2, Add(iNum1, iNum2));
    printf("Subtract(%d, %d) = %d\n", iNum1, iNum2, Subtract(iNum1, iNum2));
    printf("Multiply(%d, %d) = %d\n", iNum1, iNum2, Multiply(iNum1, iNum2));

    FreeLibrary(hModule);
    return 0;
}
