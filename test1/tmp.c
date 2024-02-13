#include <stdio.h>
#include <stdlib.h>

int weird_math(int n, int c) {
    if (n == 48) {
        printf("Found 48 in %d steps\n", c);
        return 1;
    } else if (n % 2 == 0) {
        printf("%d is even\n", n);
        int temp = weird_math(n / 2, c + 1);
        printf("%d got %d\n", n, temp);
        return temp + 1;
    } else {
        printf("%d is odd\n", n);
        int temp = weird_math(5 * n + 3, c + 1);
        printf("%d has %d\n", n, temp);
        return temp * 2;
    }
}

int main(void) {
    weird_math(3, 0);
    return EXIT_SUCCESS;
}

