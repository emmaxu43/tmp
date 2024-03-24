#include <stdio.h>
#include <stdlib.h>
#include "rand_story.h"
#include "provided.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <words_file>\n", argv[0]);
        return 1;
    }

    // Create a catarray_t to store categories and words
    catarray_t cats;
    if (!read_categories(argv[1], &cats)) {
        return 1;
    }

    // Print the categories and words
    printWords(&cats);
    //print_words(&cats);

    // Free allocated memory
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

