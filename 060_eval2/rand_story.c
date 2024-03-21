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

/*
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
*/


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
                free(category_name);  // Free the duplicate category name
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

/*
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
    		free(category_name); 
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
*/

// STEP3
void replace_blanks(char *story, category_t *used_words, catarray_t *cats) {
    char *blank_start = strstr(story, "_");
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
            int idx = atoi(category_name);
            if (idx > 0 && idx <= used_words->n_words) {
                replacement = used_words->words[used_words->n_words - idx];
            } else {
                fprintf(stderr, "Error: Invalid backreference '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        } else {
            replacement = chooseWord(category_name, cats);
            if (replacement != NULL) {
                used_words->words = realloc(used_words->words, (used_words->n_words + 1) * sizeof(char *));
                used_words->words[used_words->n_words] = strdup(replacement);
                used_words->n_words++;
            }
        }

        memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, strlen(replacement));

        blank_start = strstr(blank_start + strlen(replacement), "_");
    }
}


// Add a new function to replace all blanks with a single word
void replace_blanks_with_word(char *story, const char *replacement) {
    char *blank_start = strstr(story, "_");
    while (blank_start != NULL) {
        char *blank_end = strstr(blank_start + 1, "_");
        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            exit(EXIT_FAILURE);
        }

        memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, strlen(replacement));

        blank_start = strstr(blank_start + strlen(replacement), "_");
    }
}



// STEP4
const char *choose_word_from_category(char *category, catarray_t *cats, bool no_reuse) {
    for (size_t i = 0; i < cats->n; i++) {
        if (strcmp(cats->arr[i].name, category) == 0) {
            if (cats->arr[i].n_words == 0) {
                fprintf(stderr, "Error: Category '%s' has no words\n", category);
                exit(EXIT_FAILURE);
            }

            size_t idx = rand() % cats->arr[i].n_words;
            const char *word = cats->arr[i].words[idx];

            if (no_reuse) {
                // Remove the chosen word from the category's word list
                cats->arr[i].words[idx] = cats->arr[i].words[cats->arr[i].n_words - 1];
                cats->arr[i].n_words--;
            }

            return word;
        }
    }

    fprintf(stderr, "Error: Category '%s' not found\n", category);
    exit(EXIT_FAILURE);
}



void replace_blanks_no_reuse(char *story, category_t *used_words, catarray_t *cats, bool no_reuse) {
    char *blank_start = strstr(story, "_");
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
            int idx = atoi(category_name);
            if (idx > 0 && idx <= used_words->n_words) {
                replacement = used_words->words[used_words->n_words - idx];
            } else {
                fprintf(stderr, "Error: Invalid backreference '%s'\n", category_name);
                exit(EXIT_FAILURE);
            }
        } else {
            replacement = choose_word_from_category(category_name, cats, no_reuse);
            if (replacement != NULL) {
                used_words->words = realloc(used_words->words, (used_words->n_words + 1) * sizeof(char *));
                used_words->words[used_words->n_words] = strdup(replacement);
                used_words->n_words++;
            }
        }

        memmove(blank_start + strlen(replacement), blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, strlen(replacement));

        blank_start = strstr(blank_start + strlen(replacement), "_");
    }
}
