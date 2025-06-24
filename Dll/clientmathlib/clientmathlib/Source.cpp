#include <windows.h>
#include <iostream>
#include <MathLib.h>

int main() 
{
    int iNum1, iNum2;
    printf("Enter Num1 & Num2 : ");
    scanf_s("%d %d", &iNum1, &iNum2);

    printf("Add(%d, %d) = %d\n", iNum1, iNum2, Add(iNum1, iNum2));
    printf("Subtract(%d, %d) = %d\n", iNum1, iNum2, Subtract(iNum1, iNum2));
    printf("Multiply(%d, %d) = %d\n", iNum1, iNum2, Multiply(iNum1, iNum2));

    return 0;
}