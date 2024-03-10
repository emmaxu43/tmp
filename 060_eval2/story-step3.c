#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rand_story.h"
#include "provided.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <words_file> <story_file>\n", argv[0]);
        return 1;
    }

    catarray_t cats;
    if (!read_categories(argv[1], &cats)) {
        fprintf(stderr, "Error reading categories and words\n");
        return 1;
    }

    char *story;
    if (parse_story(argv[2], &story)) {
        fprintf(stderr, "Error parsing story file\n");
        return 1;
    }

    // Create a category to store used words
    category_t used_words = {"used_words", NULL, 0};

    replace_blanks(story, &used_words, &cats);

    printf("%s\n", story);

    // Free memory
    free(story);
    free(used_words.words);
    for (size_t i = 0; i < cats.n; i++) {
        free(cats.arr[i].name);
        for (size_t j = 0; j < cats.arr[i].n_words; j++) {
            free(cats.arr[i].words[j]);
        }
        free(cats.arr[i].words);
    }
    free(cats.arr);

    return 0;
}
