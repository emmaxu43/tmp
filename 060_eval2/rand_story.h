#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"


/* File Operation */

FILE * openFile(char * filename);
void closeFile(FILE * f);

/* Initialize */

category_t * initCat();
catarray_t * initCatsArr();

/* free */

void freeCategory(category_t * category);
void freeCategories(catarray_t * catArray);

void parseNprint(const char * story_file);
int isValidInt(char * str);
const char * getUniWord(char * category, catarray_t * cats, category_t * used_words);
int checkWord(const char * word, category_t * category);
void removeWordFromCategory(char * category, const char * word, catarray_t * cats);
const char * getUniWord(char * category, catarray_t * cats, category_t * used_words);
void parseStory(char * story_file, catarray_t * cats, int step, int no_reuse);

char * findCat(char * line);
char * findWord(char * line);
int checkCat(const char * cat, catarray_t * catArray);
catarray_t * readWordsFile(FILE * f);

#endif

