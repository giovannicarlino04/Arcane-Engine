#ifndef LOGGING_H
#define LOGGING_H

#include <Windows.h>

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API void AEMessageBox(const char* message, const char* caption);

#ifdef __cplusplus
}
#endif

#endif  //LOGGING_H