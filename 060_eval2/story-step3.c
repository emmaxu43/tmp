#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s words_category.txt story_template.txt\n", argv[0]);
    return EXIT_FAILURE;
  }

  char * words_file = argv[1];
  char * story_file = argv[2];

  FILE *f = fopen(words_file, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Could not open words template file\n");
    exit(EXIT_FAILURE);
  }

  catarray_t * cats = read_words(f,3,0);

  parse_story(story_file, cats, 3, 0);  // no reuse words flag (no use to be false).

  free_catarray(cats,3,0);

  return EXIT_SUCCESS;
}

