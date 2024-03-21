#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rand_story.h"


int parse_story(const char *filename, char **story) {
    // printf("Trying to parse %s\n",filename);	
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


void replace_blanks(char **story, const char* replacement) {
    printf("Starting word replacement\n");
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

        // Replace the placeholder with the replacement word
        memmove(blank_start + replacement_length, blank_end + 1, strlen(blank_end + 1) + 1);
        memcpy(blank_start, replacement, replacement_length);

        // Find the next placeholder
        blank_start = strchr(blank_start + replacement_length, '_');
    }
}


/*
void replace_blanks(char *story, const char *replacement) {
    printf("Starting word replacement\n");
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


/*
void replace_blanks(char *story, const char *replacement) {
    printf("Starting word replacement\n");
    char *blank_start = strchr(story, '_');
    char *blank_end;

    while (blank_start != NULL) {
        blank_end = strchr(blank_start + 1, '_');
        if (blank_end == NULL) {
            fprintf(stderr, "Error: Missing closing underscore\n");
            return;
        }

        // Replace the blank with the replacement word
        strncpy(blank_start, replacement, blank_end - blank_start + 1);
        blank_start = strchr(blank_end + 1, '_');
    }
}
*/
