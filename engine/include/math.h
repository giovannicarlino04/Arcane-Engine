#ifndef MATH_H
#define MATH_H

#include "defines.h"

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

global_variable float PI32 = 3.14159265f;

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API double add(double a, double b);
ARCANE_API double subtract(double a, double b);
ARCANE_API double multiply(double a, double b);
ARCANE_API double divide(double a, double b);
ARCANE_API double pow(double base, int exponent);
ARCANE_API double sine(double x);
ARCANE_API double cosine(double x);
ARCANE_API unsigned long long factorial(int n);

#ifdef __cplusplus
}
#endif

#endif // MATH_H
