#include "pch.h"
#include "MathLib.h"


int MathLib_Add(int iOperand1, int iOperand2)
{
    int iResult = iOperand1 + iOperand2;
    printf("[MathLib] Adding %d + %d = %d\n", iOperand1, iOperand2, iResult);
    return iResult;
}

int MathLib_Subtract(int iOperand1, int iOperand2)
{
    int iResult = iOperand1 - iOperand2;
    printf("[MathLib] Subtracting %d - %d = %d\n", iOperand1, iOperand2, iResult);
    return iResult;
}

int MathLib_Multiply(int iOperand1, int iOperand2)
{
    int iResult = iOperand1 * iOperand2;
    printf("[MathLib] Multiplying %d * %d = %d\n", iOperand1, iOperand2, iResult);
    return iResult;
}

int MathLib_Divide(int iOperand1, int iOperand2)
{
    if (iOperand2 == 0)
    {
        fprintf(stderr, "Error : Not use zero as deviser");
        return INT_MIN;
    }

    int iResult = iOperand1 / iOperand2;
    printf("[MathLib] Dividing %d / %d = %d\n", iOperand1, iOperand2, iResult);

    return iResult;
}   