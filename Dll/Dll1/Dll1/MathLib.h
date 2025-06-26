#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

extern "C" MATHLIBRARY_API int Add(int iIntemA, int iItemB);
extern "C" MATHLIBRARY_API int Subtract(int iIntemA, int iItemB);
extern "C" MATHLIBRARY_API int Multiply(int iIntemA, int iItemB);


























/*
1. Without extern "C"
	MATHLIBRARY_API int Add(int a, int b);  // C++ style export
		The compiler "mangles" (changes) the function name to include type information

		Your Add function might become ?Add@@YAHHH@Z internally

		Other programs won't find your function when calling GetProcAddress("Add")

2. With extern "C": 
	extern "C" MATHLIBRARY_API int Add(int a, int b);  // C style export
		Name stays exactly as Add

		Works perfectly with GetProcAddress()

		Compatible with C and C++ callers


3. What Happens Without MATHLIBRARY_API?
	int Add(int a, int b);  // Normal function declaration
		The function won't be exported from your DLL

		Other programs can't see or use it

		GetProcAddress() will return NULL

4. With the macro:
	MATHLIBRARY_API int Add(int a, int b);  // Proper export
		The function is publicly visible from the DLL

		Can be loaded with GetProcAddress()

		Shows up in the DLL's export table


	
*/


