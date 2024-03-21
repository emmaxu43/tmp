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


void replace_blanks(char *story, const char *replacement) {
    char *blank_start = strchr(story, '_');
    char *blank_end;

    while (blank_start != NULL) {
        blank_end = strchr(blank_start + 1, '_');
        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            return;
        }

        // Replace the placeholder with the replacement word
        memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, strlen(replacement));

        // Find the next placeholder
        blank_start = strchr(blank_start + strlen(replacement), '_');
    }
}


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


