#ifndef MATH_H
#define MATH_H

#include "defines.h"

global_variable float PI32 = 3.14159265f;

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double pow(double base, int exponent);
double sine(double x);
double cosine(double x);
unsigned long long factorial(int n);

#endif // MATH_H
