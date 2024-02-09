#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
    if ((array == NULL) || (n==0)) {
        return 0;
    }

    if (n == 1) {
        return 1;
    }

    int tmp = 1;
    size_t i;
    size_t max = 0;
    for (i = 0; i < n - 1; ++i) {
        if (array[i] < array[i + 1]) {
            ++tmp;
        } else {
            if (max < tmp) {
                max = tmp;
            }
            tmp = 1;
        }
    }

    if (max < tmp) {
        max = tmp;
    }

    return max;
}


