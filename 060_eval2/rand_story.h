#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"
#include <stdbool.h>

int parse_story(const char *filename, char **story);
//void replace_blanks(char *story, const char *replacement);
int read_categories(const char *filename, catarray_t *cats);
void replace_blanks(char *story, category_t *used_words, catarray_t *cats);
void replace_blanks_with_word(char *story, const char *replacement);
const char *choose_word_from_category(char *category, catarray_t *cats, bool no_reuse);
void replace_blanks_no_reuse(char *story, category_t *used_words, catarray_t *cats, bool no_reuse);

#endif
