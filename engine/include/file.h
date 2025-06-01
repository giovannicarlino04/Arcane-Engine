#ifndef FILE_H
#define FILE_H
#include <cstdint>
#include <windows.h>
#include <cassert>
#include <cstdio>

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API FILE *AEReadEntireFile(const char *fileName);
ARCANE_API void AEFreeFileMemory(void *memory);
ARCANE_API bool AEWriteEntireFile(char *fileName, uint32_t memorySize, void* Memory);

#ifdef __cplusplus
}
#endif

#endif