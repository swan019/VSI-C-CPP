#include "Header.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <numA numB>\n", argv[0]);
        return 0;
    }
    int iNumA = std::stoi(argv[1]);
    int iNumB = std::stoi(argv[2]);

    int iAdditionResult = MathLib_Add(iNumA, iNumB);
    int iSubtractResult = MathLib_Subtract(iNumA, iNumB);
    int iMultiplyResult = MathLib_Multiply(iNumA, iNumB);
    int iDivideResult = MathLib_Divide(iNumA, iNumB);

    printf("Add: %d\n", iAdditionResult);
    printf("Subtract: %d\n", iSubtractResult);
    printf("Multiply: %d\n", iMultiplyResult);
    return 0;
}