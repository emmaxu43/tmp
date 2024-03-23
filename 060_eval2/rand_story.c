#include "rand_story.h"

/* file open */

FILE * openFile(char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Could not Open File");
    exit(EXIT_FAILURE);
  }
  return f;
}

/* file close */

void closeFile(FILE * f) {
  if (fclose(f) != 0) {
    perror("Could not Close File");
    exit(EXIT_FAILURE);
  }
}

/* initialize category */

category_t * initCat() {
  category_t * cat = malloc(sizeof(*cat));
  cat->n_words = 0;
  cat->name = NULL;
  cat->words = NULL;
  return cat;
}

/* initialize categories array */

catarray_t * initCatsArr() {
  catarray_t * cats = malloc(sizeof(*cats));
  cats->n = 0;
  cats->arr = NULL;
  return cats;
}

/* free functions */

void freeCategory(category_t * cat) {
  for (size_t i = 0; i < cat->n_words; i++) {
    free(cat->words[i]);
  }
  free(cat->words);
  free(cat);
}

void freeCategories(catarray_t * catArray) {
  for (size_t i = 0; i < catArray->n; i++) {
    for (size_t j = 0; j < catArray->arr[i].n_words; j++) {
      free(catArray->arr[i].words[j]);
    }
    free(catArray->arr[i].words);
    free(catArray->arr[i].name);
  }
  free(catArray->arr);
  free(catArray);
}

/* for step1 only */
void parseNprint(const char * story_file) {
  FILE * f = fopen(story_file, "r");
  if (f == NULL) {
    perror("Could not Open File");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, f)) != -1) {
    char * current = line;                //track current position
    char * start = strchr(current, '_');  // 1st underscore
    while (start) {
      char * end = strchr(start + 1, '_');  // find the next 1st underscore
      if (end == NULL) {
        perror("Underscores Mismatched on the Same Line");
        exit(EXIT_FAILURE);
      }
      // replace with 'cat'
      const char * replace = chooseWord(NULL, NULL);    // cats->NULL, return "cat"
      for (char * ptr = current; ptr < start; ptr++) {  // print everything before strat
        printf("%c", *ptr);
      }
      printf("%s", replace);
      current = end + 1;
      start = strchr(current, '_');
    }
    printf("%s", current);  // print everything remaining
  }
  free(line);
  fclose(f);
}

/* this function is to check if a string is a valid integer */

int isValidInt(char * str) {
  while (*str) {
    if (!isdigit((unsigned char)*str)) {
      return 0;
    }
    str++;
  }
  return 1;
}
/* this function is to check if a word is inside a cetegory */

int checkWord(const char * word, category_t * category) {
  for (size_t i = 0; i < category->n_words; i++) {
    if (strcmp(word, category->words[i]) == 0) {
      return 1;  // word found
    }
  }
  return 0;  // word not found
}

/* get a unique word, not previously used */

const char * getUniWord(char * category, catarray_t * cats, category_t * used_words) {
  const char * word = chooseWord(category, cats);
  while (checkWord(word, used_words)) {
    word = chooseWord(category, cats);
  }
  used_words->words =
      realloc(used_words->words, (used_words->n_words + 1) * sizeof(*used_words->words));
  used_words->words[used_words->n_words] = strdup(word);
  used_words->n_words++;

  return word;
}

/* find the specified word within the specified category and remove it  */

void removeWordFromCategory(char * category, const char * word, catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cats->arr[i].name, category) == 0) {  //category found
      for (size_t j = 0; j < cats->arr[i].n_words; j++) {
        if (strcmp(cats->arr[i].words[j], word) == 0) {  //word found
          cats->arr[i].words[j] = NULL;
          // shift position to the left
          for (size_t k = j; k < cats->arr[i].n_words - 1; k++) {
            cats->arr[i].words[k] = cats->arr[i].words[k + 1];
          }
          cats->arr[i].n_words--;  //number of words decrese
          // reallocate the words array to its new size
          cats->arr[i].words = realloc(
              cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
          return;
        }
      }
    }
  }
  fprintf(stderr, "Error: Word '%s' not Found in Category '%s'\n", word, category);
  exit(EXIT_FAILURE);
}

/* parse story */

void parseStory(char * story_file, catarray_t * cats, int step, int no_reuse) {
  FILE * f = openFile(story_file);

  char * line = NULL;
  size_t sz = 0;
  ssize_t read;
  category_t used_words;
  used_words.n_words = 0;
  used_words.words = NULL;

  while ((read = getline(&line, &sz, f) != -1)) {
    char * current = line;
    char * start = strchr(current, '_');  // locate the 1st underscore
    while (start) {
      char * end = strchr(start + 1, '_');
      if (end == NULL) {
        perror("Underscore Mismatched On the  Same Line\n");
        exit(EXIT_FAILURE);
      }
      size_t len = end - start - 1;  // blank enclosed by underscore [start+1,end]
      char temp[len + 1];
      strncpy(temp, start + 1, len);
      temp[len] = '\0';

      //printf("Debug: Checking Category: '%s'\n", temp);  //debugger

      const char * wordToUse;

      if (isValidInt(temp)) {
        size_t index = atoi(temp);
        if (index <= 0 || index > used_words.n_words) {
          fprintf(stderr, "Error: Invalid reference '%s'\n", temp);
          exit(EXIT_FAILURE);
        }
        wordToUse = used_words.words[used_words.n_words - index];
      }
      else if (cats != NULL && checkCat(temp, cats)) {
        if (step == 4 && no_reuse) {
          wordToUse = getUniWord(temp, cats, &used_words);
        }
        else {
          wordToUse = chooseWord(temp, cats);
        }
        used_words.words = realloc(used_words.words,
                                   (used_words.n_words + 1) * sizeof(*used_words.words));
        used_words.words[used_words.n_words] = (char *)wordToUse;
        used_words.n_words++;
        if (step == 4 && no_reuse) {
          removeWordFromCategory(temp, wordToUse, cats);
        }
      }
      else {
        fprintf(stderr, "Error: Invalid Category '%s'\n", temp);
        exit(EXIT_FAILURE);
      }

      for (char * ptr = current; ptr < start; ptr++) {
        printf("%c", *ptr);  //print everything before start
      }
      printf("%s", wordToUse);
      current = end + 1;
      start = strchr(current, '_');  //initialize for the next blank
    }
    printf("%s", current);
  }
  //free used_words
  for (size_t i = 0; i < used_words.n_words; i++) {
    if (step == 4 && no_reuse) {
      free(used_words.words[i]);
    }
    used_words.words[i] = NULL;
  }
  free(used_words.words);

  free(line);
  closeFile(f);
}
/* find the categories in the line */

char * findCat(char * line) {
  char * colon = strchr(line, ':');  // locate the colon which seperate category and word
  if (colon == NULL) {
    perror("Invalid Format: must have a colon");
    exit(EXIT_FAILURE);
  }
  int len_cat = colon - line;                         // the length of category
  char * cat = malloc((len_cat + 1) * sizeof(*cat));  //with null terminator
  strncpy(cat, line, len_cat);
  cat[len_cat] = '\0';  //place null terminator to seperate
  return cat;
}

/* find word in the line */

char * findWord(char * line) {
  char * colon = strchr(line, ':');  // locate the colon which seperate category and word
  char * newLine = strchr(line, '\n');  // locate the newline character
  colon++;                              // start at colon ++
  int len_word = newLine - colon;
  char * word = malloc((len_word + 1) * sizeof(*word));
  strncpy(word, colon, len_word);
  word[len_word] = '\0';
  return word;
}

/* check if category exists */

int checkCat(const char * cat, catarray_t * catArray) {
  //  if (catArray == NULL) {
  //return 0;
  //}
  for (size_t i = 0; i < catArray->n; i++) {
    if (strcmp(catArray->arr[i].name, cat) == 0) {
      return 1;  // found
    }
  }
  return 0;  // not found
}

/* read the words file */

catarray_t * readWordsFile(FILE * f) {
  catarray_t * cats = initCatsArr();

  size_t sz = 0;
  char * line = NULL;
  while (getline(&line, &sz, f) != -1) {
    char * cat = findCat(line);
    char * word = findWord(line);

    if (checkCat(cat, cats) == 0) {  // cat not in the list
      cats->arr = realloc(
          cats->arr, (cats->n + 1) * sizeof(*cats->arr));  // realloc for new category
      cats->arr[cats->n].name = cat;                       //assign new cat to the name
      cats->arr[cats->n].n_words = 1;                      // the number now is 1
      cats->arr[cats->n].words = NULL;
      cats->arr[cats->n].words =
          realloc(cats->arr[cats->n].words,
                  cats->arr[cats->n].n_words * (sizeof(*cats->arr[cats->n].words)));
      cats->arr[cats->n].words[cats->arr[cats->n].n_words - 1] =
          word;  //assign word to words array
      cats->n++;
    }
    else if (checkCat(cat, cats) == 1) {  // cat exsists, add word to words array
      for (size_t i = 0; i < cats->n; i++) {
        if (strcmp(cat, cats->arr[i].name) == 0) {
          for (size_t j = 0; j < cats->arr[i].n_words; j++) {
            if (strcmp(word, cats->arr[i].words[j]) == 0) {
              free(word);  // see if the word already exists
            }
          }
          cats->arr[i].n_words++;
          cats->arr[i].words = realloc(
              cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
          cats->arr[i].words[cats->arr[i].n_words - 1] = word;
        }
      }
      free(cat);
    }
    free(line);
    line = NULL;  //initialize for the next line
    word = NULL;
  }
  free(line);
  closeFile(f);
  return cats;
}

