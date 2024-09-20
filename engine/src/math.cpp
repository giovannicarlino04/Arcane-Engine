#include "math.h"

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        // Handle division by zero
        return 0; // or some error code
    }
    return a / b;
}

unsigned long long factorial(int n) {
    if (n == 0) return 1;
    unsigned long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

double pow(double base, int exponent) {
    double result = 1.0;
    int positive_exponent = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < positive_exponent; i++) {
        result *= base;
    }
    
    if (exponent < 0) {
        result = 1.0 / result;
    }
    
    return result;
}

// Sine function using Taylor series
double sine(double x) {
    double result = 0;
    const int TERMS = 10;  // Number of terms in the Taylor series
    for (int n = 0; n < TERMS; n++) {
        double term = multiply(multiply(pow(-1, n), pow(x, 2 * n + 1)), 1.0 / factorial(2 * n + 1));
        result += term;
    }
    return result;
}

// Cosine function using Taylor series
double cosine(double x) {
    double result = 0;
    const int TERMS = 10;  // Number of terms in the Taylor series
    for (int n = 0; n < TERMS; n++) {
        double term = multiply(multiply(pow(-1, n), pow(x, 2 * n)), 1.0 / factorial(2 * n));
        result += term;
    }
    return result;
}
