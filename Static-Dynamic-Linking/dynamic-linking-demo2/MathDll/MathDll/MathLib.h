#pragma once

#define MATHLIBRARY_API __declspec(dllexport)



extern "C" MATHLIBRARY_API int MathLib_Add(int a, int b);
extern "C" MATHLIBRARY_API int MathLib_Subtract(int a, int b);
extern "C" MATHLIBRARY_API int MathLib_Multiply(int a, int b);
extern "C" MATHLIBRARY_API int MathLib_Divide(int a, int b); 


