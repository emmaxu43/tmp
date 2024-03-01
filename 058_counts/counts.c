#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
  //WRITE ME
  counts_t *content = malloc(sizeof(counts_t));
  content->array = NULL;
  content->size = 0;
  content->unknown = 0;
  return content;
}

void addCount(counts_t * c, const char * name) {
  //WRITE ME
  if (name == NULL) {
    c->unknown++;
    return;
  }

  for (size_t i = 0; i < c->size; ++i) {
    if (strcmp(c->array[i].content, name) == 0) {
      c->array[i].count++;
      return;
    }
  }

  c->array = realloc(c->array, (c->size + 1) * sizeof(one_count_t));
  c->array[c->size].content = strdup(name);
  c->array[c->size].count = 1;
  c->size++;
}

void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (size_t i = 0; i < c->size; ++i) {
    fprintf(outFile, "%s: %d\n", c->array[i].content, c->array[i].count);
  }
  if (c->unknown > 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknown);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (size_t i = 0; i < c->size; i++) {
    free(c->array[i].content);
  }
  free(c->array);
  free(c);
}
