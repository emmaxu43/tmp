#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int retrieve_frequentest_letter(int *array) {
    int max_occurrence = 0;
    int frequentest_letterIDX = 0;
    int flag = 0;

    for (int i = 0; i < 26; ++i) {
        if (array[i] > max_occurrence) {
            max_occurrence = array[i];
            frequentest_letterIDX = i;
            flag = 1;
        }
    }

    if ((max_occurrence == 0) && (flag == 0)) {
        printf("No letter found\n");
        return EXIT_FAILURE;
    } else {
        // The current maxIDX is associated with 'e' (idx=4, but in the previous dict, it is like idx==0).
        if (frequentest_letterIDX < 5) {
            frequentest_letterIDX += 22;
        } else {
            frequentest_letterIDX -= 4;
        }

        if (frequentest_letterIDX == 26) {
            frequentest_letterIDX = 0;
        }

        printf("%d\n", frequentest_letterIDX);
    }

    return frequentest_letterIDX;
}

int frequency_counting(FILE *f) {
    int c;
    int hashTable[26] = {0};

    while ((c = fgetc(f)) != EOF) {
        // Only save letters
        if (isalpha(c)) {
            c = tolower(c);
            hashTable[c - 'a']++;
        }
    }

    int frequentest_letter_index = retrieve_frequentest_letter(hashTable);
    return frequentest_letter_index;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s inputFileName\n", argv[0]);
        return EXIT_FAILURE;
    }

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

