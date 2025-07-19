/*
 * BORROWED CODE - Type Conversion Utilities
 * This file contains utility functions for type conversion
 * borrowed from external library code.
 */

#include <stdint.h>
#include <stdlib.h>

#include "type.h"

// Convert integer to void pointer (for decimal type)
extern void *decimal(int32_t x) {
    return (void*)(intptr_t)x;
}

// Convert string to void pointer (for string type)
extern void *string(char *x) {
    return (void*)x;
}

// Convert double to void pointer (for real type) - allocates memory
extern void *real(double x) {
    double *f = (double*)malloc(sizeof(double));
    *f = x;
    return (void*)f;
}
