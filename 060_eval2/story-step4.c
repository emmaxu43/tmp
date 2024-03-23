#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] categories.txt story_template.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  int no_reuse = 0;
  char * wordsFile;
  char * storyFile;
  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    no_reuse = 1;  // Set no_reuse to true.
    wordsFile = argv[2];
    storyFile = argv[3];
  }
  else if (argc == 3) {
    wordsFile = argv[1];
    storyFile = argv[2];
  }
  else {
    fprintf(stderr, "Usage: %s [-n] words.txt story.txt \n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = openFile(wordsFile);
  catarray_t * cats = readWordsFile(f);

  parseStory(storyFile, cats, 4, no_reuse);

  freeCategories(cats);

  return EXIT_SUCCESS;
}

