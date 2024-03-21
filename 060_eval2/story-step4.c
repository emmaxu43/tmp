#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "rand_story.h"
#include "provided.h"


int main(int argc, char *argv[]) {
    bool no_reuse = false;
    char *words_file, *story_file;

    // Parse command-line arguments
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s [-n] <words_file> <story_file>\n", argv[0]);
        return 1;
    }

    int arg_index = 1;
    if (strcmp(argv[1], "-n") == 0) {
        no_reuse = true;
        arg_index++;
    }

    words_file = argv[arg_index];
    story_file = argv[arg_index + 1];

    catarray_t cats;
    if (!read_categories(words_file, &cats)) {
        fprintf(stderr, "Error reading categories and words\n");
        return 1;
    }

    char *story;
    if (parse_story(story_file, &story)) {
        fprintf(stderr, "Error parsing story file\n");
        return 1;
    }

    category_t used_words = {"used_words", NULL, 0};

    if (no_reuse) {
        replace_blanks_no_reuse(&story, &used_words, &cats, no_reuse);
    } else {
        replace_blanks(&story, &used_words, &cats);
    }

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
