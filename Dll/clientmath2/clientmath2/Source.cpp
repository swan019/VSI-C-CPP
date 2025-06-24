#include "Header.h"

typedef int (*MATHFUNC)(int, int);

int main() {

    HMODULE hModule = LoadLibraryW(fileName);
    if (hModule == NULL) {
        fprintf(stderr, "LoadLibrary failed. Error: (%lu)\n", GetLastError());

        return -1;
    }

    MATHFUNC Add = (MATHFUNC)GetProcAddress(hModule, "Add");
    MATHFUNC Subtract = (MATHFUNC)GetProcAddress(hModule, "Subtract");
    MATHFUNC Multiply = (MATHFUNC)GetProcAddress(hModule, "Multiply");

    if (!Add || !Subtract || !Multiply) {
        fprintf(stderr, "GetProcAddress failed. Error: (%lu)\n", GetLastError());
        FreeLibrary(hModule);
        return -1;
    }

    int iNum1, iNum2;
    printf("Enter Num1 & Num2 : ");
    scanf_s("%d %d", &iNum1, &iNum2);

    printf("Add(%d, %d) = %d\n", iNum1, iNum2, Add(iNum1, iNum2));
    printf("Subtract(%d, %d) = %d\n", iNum1, iNum2, Subtract(iNum1, iNum2));
    printf("Multiply(%d, %d) = %d\n", iNum1, iNum2, Multiply(iNum1, iNum2));

    FreeLibrary(hModule);
    return 0;
}