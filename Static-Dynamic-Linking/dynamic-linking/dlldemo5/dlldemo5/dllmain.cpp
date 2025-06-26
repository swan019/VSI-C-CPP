#include "pch.h"
#define MATHLIBRARY_EXPORTS

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) 
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            if (NULL == lpReserved) {
                printf("[MathLib] DLL loaded dynamically using LoadLibrary.\n");
            }
            else {
                printf("[MathLib] DLL loaded statically.\n");
            }
            break;
        case DLL_PROCESS_DETACH:
            printf("[MathLib] DLL unloaded.\n");
            break;
        case DLL_THREAD_ATTACH:
            printf("[MathLib] DLL, Thread attached.\n");
            break;
        case DLL_THREAD_DETACH:
            printf("[MathLib] DLL, Thread detach.\n");
            break;
    }
    return TRUE;
}

