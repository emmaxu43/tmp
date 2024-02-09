#include <stdio.h>
#include <stdlib.h>

//prototype
size_t maxSeq(int * array, size_t n);

void test(int * array, size_t n, size_t expected) {
    size_t result = maxSeq(array, n);
    if (result != expected) {
        printf("test failed ");
        printf("expected %lu result  %lu\n",expected,result);
        exit(EXIT_FAILURE);
    }
}

int main(void) {
  int array1[19] = {2, 3, 2, 3, 4, 3, 4, 5, 6, 7, 8, 8, 9, 10, 12, 13, 14, 15, 16};
  test(array1,19,8);

  int array2[14] = {-52, 21, 23, 25, 31, -35, -36, 41, -110, -109, -108, -107, -106, -105};
  test(array2,14,6);

  int array3[6] = {0, 0, 0, 0, 0, 0};
  test(array3,6,1);

  int array4[1] = {3};
  test(array4,1,1);
 
  int array5[6] = {0,1,2,0,1,2};
  test(array5,6,3);
 
  int array6[6] = {0,-1,-2,0,-1,-2};
  test(array6,6,2); 

  int array7[6] = {1, 2, 3, 4, 5, 6};
  test(array7,6,6);

  int *array8 = NULL;
  test(array8,0,0);



 
  printf("all tests passed.\n");
  return EXIT_SUCCESS;
}

