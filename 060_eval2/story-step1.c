#include <stdio.h>
#include <stdlib.h>
#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    perror("Usage: ./Program Inputfile\n");
    exit(EXIT_FAILURE);
  }
  char * story_file_name = argv[1];
  parse_print_story(story_file_name);

  return EXIT_SUCCESS;
}

