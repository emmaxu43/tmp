#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    exit(EXIT_FAILURE);
  }
}

int main() {
  check(0, 0, 1);
  check(3, 0, 1);
  check(4294967295, 1, 4294967295);
  check(0, 5, 0);
  check(2, 2, 4);
  check(1, 5, 1);
  return EXIT_SUCCESS;
}
