#include <stdio.h>
#include <stdlib.h>
#include "rand_story.h"
#include "provided.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <story_file>\n", argv[0]);
        return 1;
    }

    char *story = NULL;
    if (parse_story(argv[1], &story) != 0) {
        return 1;
    }

    replace_blanks_with_word(story, "cat");
    printf("%s\n", story);
    free(story);

    return 0;
}
