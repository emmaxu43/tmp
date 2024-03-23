#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s words.txt\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE * f = openFile(argv[1]);
  catarray_t * cats = readWordsFile(f);
  printWords(cats);
  freeCategories(cats);
  return EXIT_SUCCESS;
}

