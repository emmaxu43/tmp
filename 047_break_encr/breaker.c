#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int retrieve_frequentest_letter(int *array){
  int max = 0;
  int frequentest_letter = 0;
  for (int i=0; i<25;++i){
    if (array[i] > max){
      max = array[i];
      frequentest_letter = i;
    }
  }
  return frequentest_letter;
}


int frequency_counting(FILE *f){
  int c;
  int hashTable[26]={0};

  while ((c = fgetc(f)) != EOF){
    if (isalpha(c)) {
      c = tolower(c);
      hashTable[c-'a']++;
    }
  }

  int frequentest_letter = retrieve_frequentest_letter(hashTable);
  printf("%d\n", frequentest_letter); 
	  
  return frequentest_letter;
}


int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr,"Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }
 
  int key = atoi(argv[1]);
  if (key == 0) {
    fprintf(stderr,"Invalid key (%s): must be a non-zero integer\n", argv[1]);
    return EXIT_FAILURE;
  }
 
  FILE * f = fopen(argv[2], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
 
  int frequentest_letter = frequency_counting(f);

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
                            
