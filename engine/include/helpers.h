#ifndef HELPERS_H
#define HELPERS_H
#include <string.h>

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API void AECombinePath(char* destination, const char* path1, const char* path2);

#ifdef __cplusplus
}
#endif

#endif