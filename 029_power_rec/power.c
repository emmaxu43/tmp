#include <stdio.h>

// Prototype for power function
unsigned power(unsigned x, unsigned y) {
    // Base case: x to the power of 0 is 1
    if (y == 0) {
        return 1;
    }
    // Recursive case: x to the power of y is x times x to the power of (y - 1)
    else {
        return x * power(x, y - 1);
    }
}


