#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s words.txt\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE * f = open_file(argv[1]);
  catarray_t * cats = read_words(f);

  printWords(cats);

  free_catarray(cats);

  return EXIT_SUCCESS;
}
