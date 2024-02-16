#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

int retrieve_frequentest_letter(int *array) {
    int max_occurrence = 1;
    int frequentest_letterIDX = 0;

    for (int i = 1; i < 26; ++i) {
        if (array[i] > max_occurrence) {
            max_occurrence = array[i]; //array is like a hashtable, storing the occurrence number of each letter;
            frequentest_letterIDX = i;
        }
    }
    // the current maxIDX is associated with 'e' (idx=4, but in the previous dict, it is like idx==0).
    if (frequentest_letterIDX<5){
	frequentest_letterIDX+=22;
	}
    else {
	frequentest_letterIDX-=4;
	}

    return frequentest_letterIDX;
}

int frequency_counting(FILE *f) {
    int c;
    int hashTable[26] = {0};

    while ((c = fgetc(f)) != EOF) {
        if (isalpha(c)) {
            c = tolower(c);
            hashTable[c - 'a']++;
        }
    }

    int frequentest_letter_index = retrieve_frequentest_letter(hashTable);
    printf("%d\n", frequentest_letter_index);

    return frequentest_letter_index;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s inputFileName\n", argv[0]);
        return EXIT_FAILURE;
    }
    /*
    int key = atoi(argv[1]);
    if (key == 0) {
      fprintf(stderr,"Invalid key (%s): must be a non-zero integer\n", argv[1]);
      return EXIT_FAILURE;
    }
    */

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    int frequentest_letter_index = frequency_counting(f);

    if (fclose(f) != 0) {
        perror("Failed to close the input file!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

