#pragma once
#include "pch.h"

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

extern "C" MATHLIBRARY_API int Add(int iIntemA, int iItemB);
extern "C" MATHLIBRARY_API int Subtract(int iIntemA, int iItemB);
extern "C" MATHLIBRARY_API int Multiply(int iIntemA, int iItemB);