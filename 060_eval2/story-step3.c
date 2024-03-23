#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s words.txt story.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  char * wordsFile = argv[1];
  char * storyFile = argv[2];

  FILE * f = open_file(wordsFile);
  catarray_t * cats = read_words(f);

  parse_story(storyFile, cats, 3, 0);  // no reuse

  free_catarray(cats);

  return EXIT_SUCCESS;
}

