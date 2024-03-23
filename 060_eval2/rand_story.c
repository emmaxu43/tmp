#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rand_story.h"
#include "provided.h"
#include <stdbool.h>

// STEP1
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



// STEP2
int read_categories(const char *filename, catarray_t *cats) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 0;
    }

    cats->n = 0;
    cats->arr = NULL;

    char *line = NULL;
    size_t line_length = 0;
    ssize_t read;

    while ((read = getline(&line, &line_length, file)) != -1) {
        char *colon = strchr(line, ':');
        if (colon == NULL) {
            fprintf(stderr, "Error: Invalid format in file %s\n", filename);
            fclose(file);
            free(line);
            return 0;
        }

        // Remove trailing newline character
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        // Set category name
        *colon = '\0';  // Null-terminate the category name at the colon
        char *category_name = strdup(line);

        // Check if category already exists
        int category_index = -1;
        for (size_t i = 0; i < cats->n; i++) {
            if (strcmp(cats->arr[i].name, category_name) == 0) {
                category_index = i;
                free(category_name); 
                break;
            }
        }

        // If category doesn't exist, create a new one
        if (category_index == -1) {
            cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(category_t));
            if (cats->arr == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                free(line);
                free(category_name);  // Free the category name if memory allocation fails
                return 0;
            }
            cats->arr[cats->n].name = category_name;
            cats->arr[cats->n].n_words = 0;
            cats->arr[cats->n].words = NULL;
            category_index = cats->n;
            cats->n++;
        }

        // Set words
        char *word = strtok(colon + 1, " \t\n");
        while (word != NULL) {
            // Allocate memory for the new word
            cats->arr[category_index].words = realloc(cats->arr[category_index].words, (cats->arr[category_index].n_words + 1) * sizeof(char *));
            if (cats->arr[category_index].words == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                free(line);
                return 0;
            }

            cats->arr[category_index].words[cats->arr[category_index].n_words] = strdup(word);
            if (cats->arr[category_index].words[cats->arr[category_index].n_words] == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                free(line);
                return 0;
            }

            word = strtok(NULL, " \t\n");
            cats->arr[category_index].n_words++;
        }
    }

    fclose(file);
    free(line);
    return 1;
}

// STEP3
void replace_blanks(char *story, catarray_t *cats) {
    char *blank_start = strstr(story, "_");
    category_t used_words = {NULL, 0, 0};

    while (blank_start != NULL) {
        char *blank_end = strstr(blank_start + 1, "_");

        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            exit(EXIT_FAILURE);
        }

        size_t category_len = blank_end - blank_start - 1;
        char *category_name = strndup(blank_start + 1, category_len);

        const char *replacement;

        if (isdigit(category_name[0])) {
            // Handle backreferences
            int idx = strtol(category_name, NULL, 10);
            if (idx > 0 && idx <= used_words.n_words) {
                replacement = used_words.words[used_words.n_words - idx];
            } else {
                fprintf(stderr, "Error: Invalid backreference '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        } else {
            // Choose a random word from the named category
            replacement = chooseWord(category_name, cats);
            if (replacement != NULL) {
                used_words.words = realloc(used_words.words, (used_words.n_words + 1) * sizeof(char *));
                used_words.words[used_words.n_words] = strdup(replacement);
                used_words.n_words++;
            } else {
                fprintf(stderr, "Error: Category '%s' not found\n", category_name);
                exit(EXIT_FAILURE);
            }
        }

        size_t blank_len = blank_end - blank_start + 1;
        size_t replacement_len = strlen(replacement);

        // Resize the story buffer if necessary
        size_t story_len = strlen(story);
        size_t new_len = story_len - blank_len + replacement_len;
        story = realloc(story, new_len + 1);

        // Move the remaining story content to make room for the replacement
        memmove(blank_start + replacement_len, blank_end + 1, story_len - (blank_end - story) + 1);
        // Copy the replacement word into the blank space
        memcpy(blank_start, replacement, replacement_len);

        free(category_name);
        blank_start = strstr(blank_start + replacement_len, "_");
    }

    // Free the memory allocated for used_words
    for (size_t i = 0; i < used_words.n_words; i++) {
        free(used_words.words[i]);
    }
    free(used_words.words);
}

/*
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

        if (isdigit(category_name[0])) {
            // Handle backreferences
            int idx = strtol(category_name, NULL, 10);
            if (idx > 0 && idx <= used_words.n_words) {
                replacement = used_words.words[used_words.n_words - idx];
            } else {
                fprintf(stderr, "Error: Invalid backreference '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        } else {
            // Choose a random word from the named category
            replacement = chooseWord(category_name, cats);
            if (replacement != NULL) {
                used_words.words = realloc(used_words.words, (used_words.n_words + 1) * sizeof(char *));
                used_words.words[used_words.n_words] = strdup(replacement);
                used_words.n_words++;
            } else {
                fprintf(stderr, "Error: Category '%s' not found\n", category_name);
                exit(EXIT_FAILURE);
            }
        }


        // Replace the blank with the chosen word
        memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, strlen(replacement));

        blank_start = strstr(blank_start + strlen(replacement), "_");
    }

    // Free the memory allocated for used_words
    for (size_t i = 0; i < used_words.n_words; i++) {
        free(used_words.words[i]);
    }
    free(used_words.words);
}
*/



