#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "provided.h"


category_t * init_category();
catarray_t * init_catarray();

// catarray_t * read_words(FILE * f);
catarray_t * read_words(FILE * f, int step, int no_reuse);

char * find_category(char * line);
char * find_word(char * line);
void parse_print_story(const char * story_file);  // STEP1 only
int is_valid_int(char * str);
int in_catarray(const char * word, catarray_t * catarray);
const char * get_unique_word(char * category, catarray_t * cats, category_t * used);
void remove_word(const char * category, const char * word, catarray_t * cats);
void parse_story(char * story_file, catarray_t * cats, int step, int no_reuse);

void free_category(category_t * cat);
void free_catarray(catarray_t * catarray, int step, int no_reus);
// void free_catarray(catarray_t * catarray);

#endif


