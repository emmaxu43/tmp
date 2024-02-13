#include <stdio.h>
#include <stdlib.h>

struct _a_struct {
    int a;
    char c;
};

typedef struct _a_struct a_struct;

a_struct f(a_struct x, int z) {
    if (z == 0) {
        printf("Base case: x = {%d, %c}, z = %d\n", x.a, x.c, z);
        return x;
    }
    x.a *= z;
    x.c -= z;
    if (z < 0) {
        printf("z = %d (less than  0)\n", z);
        z +=  2;
    } else {
        printf("z = %d (greater than 0)\n", z);
        z -= 3;
    }
    printf("x = {%d, %c}, z = %d\n", x.a, x.c, z);
    return f(x, z);
}

int main(void) {
    a_struct x;
    x.a = 10;
    x.c = 'f';
    //int y = 4;
    double y = 4.7;
    f(x, y);
    //double y = 4.7;
    printf("In main: x = {%d, %c}, y = %.1f\n", x.a, x.c, y);
    return EXIT_SUCCESS;
}


