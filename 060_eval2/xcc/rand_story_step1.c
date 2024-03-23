#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rand_story.h"


int parse_story(const char *filename, char **story) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 0;
    }

    // Determine the length of the file
    fseek(file, 0, SEEK_END);
    long file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store the file contents
    *story = malloc(file_length + 1);
    if (*story == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return 0;
    }

    // Read the file into the story buffer
    size_t bytes_read = fread(*story, 1, file_length, file);
    // Null termination
    (*story)[bytes_read] = '\0';

    if (bytes_read < file_length) {
        fprintf(stderr, "Error reading file %s\n", filename);
        free(*story);
        fclose(file);
	return 1;
    }

    fclose(file);
    return 0;
}

void replace_blanks(char *story, catarray_t *cats) {
    char *blank_start = strstr(story, "_");
    category_t used_words = {NULL, 0, 0};

    while (blank_start != NULL) {
        char *blank_end = strstr(blank_start + 1, "_");

        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            exit(EXIT_FAILURE);
        }

        char category_name[blank_end - blank_start];
        strncpy(category_name, blank_start + 1, blank_end - blank_start - 1);
        category_name[blank_end - blank_start - 1] = '\0';

        const char *replacement;

        if (cats == NULL) {
            // Replace blank with the word "cat"
            replacement = "cat";
            memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
            memcpy(blank_start, replacement, strlen(replacement));
        } else if (isdigit(category_name[0])) {
            int idx = atoi(category_name);
            if (idx > 0 && idx <= used_words.n_words) {
                replacement = used_words.words[used_words.n_words - idx];
                memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
                memcpy(blank_start, replacement, strlen(replacement));
            } else {
                fprintf(stderr, "Error: Invalid backreference '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        } else {
            replacement = chooseWord(category_name, cats);
            if (replacement != NULL) {
                used_words.words = realloc(used_words.words, (used_words.n_words + 1) * sizeof(char *));
                used_words.words[used_words.n_words] = strdup(replacement);
                used_words.n_words++;
                memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
                memcpy(blank_start, replacement, strlen(replacement));
            } else {
                fprintf(stderr, "Error: Invalid category name '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        }

        blank_start = strstr(blank_start + strlen(replacement), "_");
    }

    // Free the memory allocated for used_words
    for (size_t i = 0; i < used_words.n_words; i++) {
        free(used_words.words[i]);
    }
    free(used_words.words);
}
                          

/*
void replace_blanks(char **story, const char* replacement) {
    char *blank_start = strchr(*story, '_');
    char *blank_end;

    while (blank_start != NULL) {
        blank_end = strchr(blank_start + 1, '_');
        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            return;
        }

        size_t placeholder_length = blank_end - blank_start - 1;
        size_t replacement_length = strlen(replacement);

        // Reallocate memory if necessary
        if (replacement_length > placeholder_length) {
            size_t story_length = strlen(*story);
            size_t new_length = story_length + replacement_length - placeholder_length;
            *story = realloc(*story, new_length + 1);
            if (*story == NULL) {
                fprintf(stderr, "Memory reallocation error\n");
                return;
            }
            blank_start = *story + (blank_start - *story);
            blank_end = *story + (blank_end - *story);
        }

        // Replace the placeholder with 'cat'
        memmove(blank_start + replacement_length, blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, replacement_length);

        // Find the next placeholder
        blank_start = strchr(blank_start + replacement_length, '_');
    }
}
*/


