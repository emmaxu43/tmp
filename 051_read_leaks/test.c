#include <stdio.h>
#include <stdlib.h>

int f(int n) {
  int * p = malloc(2 * sizeof(*p));
  p[0] = n;
  p[1] = n+2;
  int ans = p[0] * p[1];
  free(p);
  return ans;
}

int main(void) {
  int * p = malloc(4 * sizeof(*p));
  int * q = p;
  int ** r = &q;
  p[0] = f(1); // assign memory to p[0];
  *r = NULL; // q -> NULL
  q = malloc(2 * sizeof(*q));
  free(p); // Free p before it is assigned to new q address
  p = q;
  q = NULL;
  free(p); //Free(NULL)
  return EXIT_SUCCESS;
}
