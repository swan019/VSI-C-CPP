#include <windows.h>
#include <string>
#include <iostream>
#include "MathLib.h"
using namespace std;

int main(int argc, char* argv[])
{
	int iNum1 = stoi(argv[1]);
	int iNum2 = stoi(argv[2]);

	printf("Add(%d, %d) = %d\n", iNum1, iNum2, Add(iNum1, iNum2));
	printf("Subtract(%d, %d) = %d\n", iNum1, iNum2, Subtract(iNum1, iNum2));
	printf("Multiply(%d, %d) = %d\n", iNum1, iNum2, Multiply(iNum1, iNum2));



	return 0;
}
