#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void sortLine(FILE *f) {
    char *line = NULL;
    size_t length = 0;
    size_t i = 0;
    char **lines = NULL;

    while (getline(&line, &length, f) >= 0) {
        lines = realloc(lines, (i + 1) * sizeof(char *));
        lines[i] = strdup(line);
        ++i;
    }
    free(line);

    sortData(lines, i);

    for (size_t j = 0; j < i; ++j) {
        printf("%s", lines[j]);
        free(lines[j]);
    }
    free(lines);
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  // If no argument, read from the input and do sorting.
  if (argc == 1) {
    FILE * f = stdin;
    sortLine(f);
  }

  // If input multiple files, sort lines within each file and iterate the files.
  else {
    for (size_t i = 1; i < argc; ++i) { // i==0 is the program name.
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
        fprintf(stderr, "Error: Empty file.\n");
	exit(EXIT_FAILURE);
      }
      sortLine(f);
      if (fclose(f) != 0) {
        fprintf(stderr, "Error: The file cannot be closed.\n");
	exit(EXIT_FAILURE);
      }
    }
  }
  return EXIT_SUCCESS;
}
