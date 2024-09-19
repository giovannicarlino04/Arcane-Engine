#ifndef FILE_H
#define FILE_H
#include <cstdint>
#include <windows.h>
#include <cassert>
#include <windows.h>
#include <cstdio>

FILE *AEReadEntireFile(const char *fileName);
void AEFreeFileMemory(void *memory);
bool AEWriteEntireFile(char *fileName, uint32_t memorySize, void* Memory);
#endif