#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"



kvarray_t * readKVs(const char * fname) {
  //WRITE ME 读取键值对
  FILE *f = fopen(fname, "r");
  if (f == NULL) {
    perror("Could not open file");
    return NULL;
  }
  kvarray_t *array = malloc(sizeof(*array));
  array->dict_pair = NULL;
  array->counts = 0;
  char *line = NULL;
  size_t sz;
  while (getline(&line, &sz, f) >= 0) {
    char *equal = strchr(line, '=');
    if (equal == NULL) {
      continue;
    }
    *equal = '\0';
    equal++;
    char *newline = strchr(equal, '\n');
    if (newline != NULL) {
      *newline = '\0';
    }
    kvpair_t *pair = malloc(sizeof(*pair));
    pair->key = strdup(line);
    pair->value = strdup(equal);
    array->dict_pair = realloc(array->dict_pair, (array->counts + 1) * sizeof(*array->dict_pair));
    array->dict_pair[array->counts] = pair;
    array->counts++;
  }
  free(line);
  fclose(f);
  return array;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->counts; i++) {
    free(pairs->dict_pair[i]->key);
    free(pairs->dict_pair[i]->value);
    free(pairs->dict_pair[i]);
  }
  free(pairs->dict_pair);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->counts; i++) {
    printf("key = '%s' value = '%s'\n", pairs->dict_pair[i]->key, pairs->dict_pair[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (size_t i = 0; i < pairs->counts; i++) {
    if (strcmp(pairs->dict_pair[i]->key, key) == 0) {
      return pairs->dict_pair[i]->value;
    }
  }
  return NULL;
}

