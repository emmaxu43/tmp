#include "rand_story.h"
#include "provided.h"

// open file
FILE * open_file(const char * filename) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Could not open file");
    exit(EXIT_FAILURE);
  }
  return f;
}

// close file  
void close_file(FILE * f) {
  if (fclose(f) != 0) {
    perror("Could not close file");
    exit(EXIT_FAILURE);
  }
}

// initialize category
category_t * init_category() {
  category_t * cat = malloc(sizeof(*cat));
  cat->n_words = 0;
  cat->name = NULL;
  cat->words = NULL;
  return cat;
}

// initialize categories array
catarray_t * init_catarray() {
  catarray_t * cats = malloc(sizeof(*cats));
  cats->n = 0;
  cats->arr = NULL;
  return cats;
}

// free category
void free_category(category_t * cat) {
  for (size_t i = 0; i < cat->n_words; i++) {
    free(cat->words[i]);
  }
  free(cat->words);
  free(cat);
}

// free categories array  
void free_catarray(catarray_t * catarray) {
  for (size_t i = 0; i < catarray->n; i++) {
    for (size_t j = 0; j < catarray->arr[i].n_words; j++) {
      free(catarray->arr[i].words[j]);
    }
    free(catarray->arr[i].words);
    free(catarray->arr[i].name);
  }
  free(catarray->arr); 
  free(catarray);
}

// parse and print story (step 1)
void parse_print_story(const char * story_file) {
  FILE * f = open_file(story_file);
  
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, f)) != -1) {
    char * current = line; 
    char * start = strchr(current, '_');
    while (start) {
      char * end = strchr(start + 1, '_');
      if (end == NULL) {
        perror("Mismatched underscores on line");
        exit(EXIT_FAILURE);
      }
      
      const char * replace = chooseWord(NULL, NULL);
      for (char * ptr = current; ptr < start; ptr++) {
        printf("%c", *ptr);
      }
      printf("%s", replace);
      
      current = end + 1;
      start = strchr(current, '_');
    }
    printf("%s", current);
  }
  
  free(line);
  close_file(f);
}

// check if string is valid integer
int is_valid_int(char * str) {
  while (*str) {
    if (!isdigit((unsigned char)*str)) {
      return 0;
    }
    str++;
  }
  return 1;
}

// check if word is in category 
int in_catarray(const char * word, catarray_t * catarray) {
  for (size_t i = 0; i < catarray->n; i++) {
    if (strcmp(catarray->arr[i].name, word) == 0) {
      return 1;
    }
  }
  return 0;
}

// get unique word not previously used
const char * get_unique_word(char * category, catarray_t * cats, category_t * used) {
  const char * word = chooseWord(category, cats);
  while (in_catarray(word, cats)) {
    word = chooseWord(category, cats);
  }

  used->words =
    realloc(used->words, (used->n_words + 1) * sizeof(*used->words));
  used->words[used->n_words] = strdup(word);
  used->n_words++;

  return word;
}

// remove word from category
void remove_word(char * category, const char * word, catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cats->arr[i].name, category) == 0) {
      for (size_t j = 0; j < cats->arr[i].n_words; j++) {
        if (strcmp(cats->arr[i].words[j], word) == 0) {
          cats->arr[i].words[j] = NULL;
          
          for (size_t k = j; k < cats->arr[i].n_words - 1; k++) {
            cats->arr[i].words[k] = cats->arr[i].words[k+1];
          }
          
          cats->arr[i].n_words--;
          cats->arr[i].words = 
            realloc(cats->arr[i].words, 
                    cats->arr[i].n_words * sizeof(*cats->arr[i].words));
          return;
        }
      }        
    }
  }

  fprintf(stderr, "Error: word '%s' not found in category '%s'\n", 
          word, category);
  exit(EXIT_FAILURE);
}

// parse story
void parse_story(char * story_file, catarray_t * cats, int step, int no_reuse) {
  FILE * f = open_file(story_file);

  char * line = NULL;
  size_t sz = 0;
  ssize_t read;

  category_t used;
  used.n_words = 0;
  used.words = NULL;

  while ((read = getline(&line, &sz, f)) != -1) {
    char * current = line;
    char * start = strchr(current, '_');
    while (start) {
      char * end = strchr(start+1, '_');
      if (end == NULL) {
        perror("Mismatched underscores on line");
        exit(EXIT_FAILURE);
      }

      size_t len = end - start - 1;
      char tmp[len+1];
      strncpy(tmp, start+1, len);
      tmp[len] = '\0';

      const char * word_to_use;

      if (is_valid_int(tmp)) {
        size_t index = atoi(tmp);
        if (index <= 0 || index > used.n_words) {
          fprintf(stderr, "Error: invalid reference '%s'\n", tmp);
          exit(EXIT_FAILURE);
        }
        word_to_use = used.words[used.n_words - index];
      }
      else if (cats != NULL && in_catarray(tmp, cats)) {
        if (step == 4 && no_reuse) {
          word_to_use = get_unique_word(tmp, cats, &used);
        }
        else {
          word_to_use = chooseWord(tmp, cats);
        }

        used.words = realloc(used.words,
                            (used.n_words + 1) * sizeof(*used.words));
        used.words[used.n_words] = strdup(word_to_use);
        used.n_words++;

        if (step == 4 && no_reuse) {
          remove_word(tmp, word_to_use, cats);
        }
      }
      else {
        fprintf(stderr, "Error: invalid category '%s'\n", tmp);
        exit(EXIT_FAILURE);
      }

      for (char * ptr = current; ptr < start; ptr++) {
        printf("%c", *ptr);
      }
      printf("%s", word_to_use);

      current = end + 1;
      start = strchr(current, '_');
    }
    printf("%s", current);
  }

  free(line);
  close_file(f);

  for (size_t i = 0; i < used.n_words; i++) {
    free(used.words[i]);
  }
  free(used.words);
}

// find category in line  
char * find_category(char * line) {
  char * colon = strchr(line, ':');
  if (colon == NULL) {
    perror("Invalid format: missing colon");
    exit(EXIT_FAILURE); 
  }
  
  int cat_len = colon - line;
  char * cat = malloc((cat_len + 1) * sizeof(*cat));
  strncpy(cat, line, cat_len);
  cat[cat_len] = '\0';
  
  return cat;
}

// find word in line
char * find_word(char * line) {
  char * newline = strchr(line, '\n');
  char * colon = strchr(line, ':');

  colon++;
  int word_len = newline - colon;
  char * word = malloc((word_len + 1) * sizeof(*word));
  strncpy(word, colon, word_len);
  word[word_len] = '\0';

  return word;
}

// read words file
catarray_t * read_words(FILE * f) {
  catarray_t * cats = init_catarray();

  size_t sz = 0;
  char * line = NULL;
  while (getline(&line, &sz, f) != -1) {
    char * cat = find_category(line);
    char * word = find_word(line);

    int found = 0;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cat, cats->arr[i].name) == 0) {
        cats->arr[i].n_words++;
        cats->arr[i].words = realloc(cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
        cats->arr[i].words[cats->arr[i].n_words - 1] = word;
        found = 1;
        break;
      }
    }

    if (!found) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].n_words = 1;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = word;
      char *tmp = strdup(word);
      free(word);
      cats->arr[cats->n].words[0] = tmp;
      for (size_t i = 1; i < cats->arr[cats->n].n_words; i++) {
        char *dup_word = strdup(cats->arr[cats->n].words[i]);
        free(cats->arr[cats->n].words[i]);
        cats->arr[cats->n].words[i] = dup_word;
      }
      cats->n++;
      free(cat);
    } else {
      free(cat); // Free the category if it's not used
    }


    free(line);
    line = NULL;
  }

  free(line);
  close_file(f);

  return cats;
}


/*
catarray_t * read_words(FILE * f) {
  catarray_t * cats = init_catarray();

  size_t sz = 0;
  char * line = NULL;
  while (getline(&line, &sz, f) != -1) {
    char * cat = find_category(line);
    char * word = find_word(line);

    int found = 0;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cat, cats->arr[i].name) == 0) {
        cats->arr[i].n_words++;
        cats->arr[i].words = realloc(cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
        cats->arr[i].words[cats->arr[i].n_words - 1] = word;
        found = 1;
        break;
      }
    }

    if (found) {
      free(cat);
      // free(word); // free the word if it's not used
    } else {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].n_words = 1;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = word;
      cats->n++;
    }

    free(line);
    line = NULL;
  }

  free(line);
  close_file(f);

  return cats;
}
*/


/*
catarray_t * read_words(FILE * f) {
  catarray_t * cats = init_catarray();

  size_t sz = 0;
  char * line = NULL;
  while (getline(&line, &sz, f) != -1) {
    char * cat = find_category(line);
    char * word = find_word(line);

    int found = 0;
    for (size_t i = 0; i < cats->n; i++) {
      if (strcmp(cat, cats->arr[i].name) == 0) {
        cats->arr[i].n_words++;
        cats->arr[i].words = realloc(cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
        cats->arr[i].words[cats->arr[i].n_words - 1] = strdup(word);
        found = 1;
        break;
      }
    }

    if (found) {
      free(cat);
    } else {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].n_words = 1;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = strdup(word);
      cats->n++;
    }

    free(word);
    free(line);
    line = NULL;
  }

  free(line);
  close_file(f);

  return cats;
}
*/



