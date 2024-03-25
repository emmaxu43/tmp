#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s [-n] categories.txt story_template.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  int no_reuse = 0;
  char * words_file;
  char * story_file;

  // Determine whether input -n parameter
  if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    no_reuse = 1;  // Set no_reuse to true
    words_file = argv[2];
    story_file = argv[3];
  }
  else if (argc == 3) {
    words_file = argv[1];
    story_file = argv[2];
  }
  else {
    fprintf(stderr, "Usage: %s [-n] words_category.txt story_template.txt \n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *f = fopen(words_file, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Could not open words template file\n");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = read_words(f);

  parse_story(story_file, cats, 4, no_reuse);

  free_catarray(cats);

  return EXIT_SUCCESS;
}


