#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"
#include <stdbool.h>


int parse_story(const char *filename, char **story);
int read_categories(const char *filename, catarray_t *cats);
//void replace_blanks(char *story, catarray_t *cats);
void replace_blanks(char **story, catarray_t *cats);

#endif
