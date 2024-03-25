#include "rand_story.h"
#include "provided.h"


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

// find the associated words category
char *find_category(char *line) {
  char *colon = strchr(line, ':');
  if (colon == NULL) {
    perror("Invalid format: missing colon\n");
    exit(EXIT_FAILURE);
  }

  int cat_len = colon - line;
  char *cat = malloc((cat_len + 1) * sizeof(*cat));
  strncpy(cat, line, cat_len);
  cat[cat_len] = '\0';
  return cat;
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

    if (found) {
      free(cat);
      //free(word); //double freed
    } else {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].n_words = 1;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = word;
      cats->n++;
      //free(word);  //double freed
    }
    free(line);
    line = NULL;
  }

  free(line);
  if (fclose(f) != 0) {
    perror("Error: Could not close file\n");
    exit(EXIT_FAILURE);
  }

  return cats;
}

// check if the string is a valid non-negative integer
int is_valid_int(char * str) {
  while (*str) {
    if (!isdigit((unsigned char)*str)) {
      return 0;
    }
    str++;
  }
  return 1;
}

// check if word is in the word categories
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
void remove_word(const char *category, const char *word, catarray_t *cats) {
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

  fprintf(stderr, "Error: word '%s' not found in categories '%s'\n",
          word, category);
  exit(EXIT_FAILURE);
}

// free category
void free_category(category_t * cat) {
  for (size_t i = 0; i < cat->n_words; i++) {
    free(cat->words[i]);
  }
  free(cat->words);
  free(cat->name);
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

// parse and print story for STEP1 only
void parse_print_story(const char * story_file) {
  FILE * f = fopen(story_file, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Could not open file\n");
    exit(EXIT_FAILURE);
  }
  fseek(f, 0, SEEK_END);
  if (ftell(f) == 0) {
    fprintf(stderr, "Error: File is empty\n");
    exit(EXIT_FAILURE);
  }
  rewind(f);

  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, f)) != -1) {
    char * current = line;
    char * start = strchr(current, '_');
    while (start) {
      char * end = strchr(start + 1, '_');
      if (end == NULL) {
        fprintf(stderr, "Error: Mismatched underscores on line: %s", line);
        free(line);
        fclose(f);
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
  if (fclose(f) != 0) {
    perror("Error: Could not close file\n");
    exit(EXIT_FAILURE);
  }
}

// parse story (STEP 3 & 4)
void parse_story(char *story_file, catarray_t *cats, int step, int no_reuse) {
 FILE *f = fopen(story_file, "r");
 if (f == NULL) {
   fprintf(stderr, "Error: Could not open file\n");
   exit(EXIT_FAILURE);
 }
 fseek(f, 0, SEEK_END);
 if (ftell(f) == 0) {
   fprintf(stderr, "Error: File is empty\n");
   exit(EXIT_FAILURE);
 }
 rewind(f);

 char *line = NULL;
 size_t sz = 0;
 ssize_t read;
 char **used_words = NULL;
 int n_used_words = 0;
 while ((read = getline(&line, &sz, f)) != -1) {
   char *current = line;
   char *start = strchr(current, '_');
   while (start) {
     char *end = strchr(start + 1, '_');
     if (end == NULL) {
       fprintf(stderr, "Error: Mismatched underscores on line: %s", line);
       free(line);
       for (int i = 0; i < n_used_words; i++) {
         free(used_words[i]);
       }
       free(used_words);
       fclose(f);
       exit(EXIT_FAILURE);
     }
     size_t len = end - start - 1;
     char tmp[len + 1];
     strncpy(tmp, start + 1, len);
     tmp[len] = '\0';
     const char *word_to_use;
     int index = atoi(tmp);
     if (is_valid_int(tmp)) {
       if (index <= 0 || index > n_used_words) {
         fprintf(stderr, "Error: invalid input '%s'\n", tmp);
         free(line);
         for (int i = 0; i < n_used_words; i++) {
           free(used_words[i]);
         }
         free(used_words);
         fclose(f);
         exit(EXIT_FAILURE);
       }
       word_to_use = used_words[n_used_words - index];
       used_words = realloc(used_words, (n_used_words + 1) * sizeof(*used_words));
       used_words[n_used_words] = strdup(word_to_use);
       n_used_words++;
     }
     else if (cats != NULL && in_catarray(tmp, cats)) {
       word_to_use = chooseWord(tmp, cats);
       if (word_to_use == NULL) {
         fprintf(stderr, "No words in category '%s'\n", tmp);
         free(line);
         for (int i = 0; i < n_used_words; i++) {
           free(used_words[i]);
         }
         free(used_words);
         fclose(f);
         exit(EXIT_FAILURE);
       }
       if (no_reuse) {
         while (n_used_words > 0 && strcmp(word_to_use, used_words[n_used_words - 1]) == 0) {
           remove_word(tmp, word_to_use, cats);
           word_to_use = chooseWord(tmp, cats);
           if (word_to_use == NULL) {
             fprintf(stderr, "No more unique words in category '%s'\n", tmp);
             free(line);
             for (int i = 0; i < n_used_words; i++) {
               free(used_words[i]);
             }
             free(used_words);
             fclose(f);
             exit(EXIT_FAILURE);
           }
         }
         remove_word(tmp, word_to_use, cats);
       }
       used_words = realloc(used_words, (n_used_words + 1) * sizeof(*used_words));
       used_words[n_used_words] = strdup(word_to_use);
       n_used_words++;
     }
     else {
       fprintf(stderr, "Error: invalid category '%s'\n", tmp);
       free(line);
       for (int i = 0; i < n_used_words; i++) {
         free(used_words[i]);
       }
       free(used_words);
       fclose(f);
       exit(EXIT_FAILURE);
     }
     for (char *ptr = current; ptr < start; ptr++) {
       printf("%c", *ptr);
     }
     printf("%s", word_to_use);
     current = end + 1;
     start = strchr(current, '_');
   }
   printf("%s", current);
 }
 free(line);
 for (int i = 0; i < n_used_words; i++) {
   free(used_words[i]);
 }
 free(used_words);

 if (fclose(f) != 0) {
   perror("Error: Could not close file\n");
   exit(EXIT_FAILURE);
 }
}




