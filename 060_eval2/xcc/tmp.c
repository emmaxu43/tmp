void replace_blanks(char *story, catarray_t *cats) {
    char *blank_start = strstr(story, "_");
    category_t used_words = {NULL, 0, 0};
    char *new_story = malloc(strlen(story) + 1);
    char *new_story_ptr = new_story;

    while (blank_start != NULL) {
        // Copy the content before the blank
        strncpy(new_story_ptr, story, blank_start - story);
        new_story_ptr += blank_start - story;
        story = blank_start;

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

        // Copy the replacement word
        strncpy(new_story_ptr, replacement, strlen(replacement));
        new_story_ptr += strlen(replacement);
        story = blank_end + 1;

        blank_start = strstr(story, "_");
    }

    // Copy the remaining content after the last blank
    strcpy(new_story_ptr, story);

    // Free the memory allocated for used_words
    for (size_t i = 0; i < used_words.n_words; i++) {
        free(used_words.words[i]);
    }
    free(used_words.words);

    // Update the original story pointer to point to the new story
    strcpy(story, new_story);
    free(new_story);
}
