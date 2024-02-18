#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rotateMatrix(FILE *file) {
    int n = 10;
    char matrix[10][10];
    char line[12];
    int rowCount = 0;

    while (fgets(line, 12, file) != NULL) {
        if (strchr(line, '\n') == NULL) {
            fprintf(stderr, "Input line is not terminated correctly\n");
            return EXIT_FAILURE;
        }
        rowCount++;

        if (rowCount > 10) {
            fprintf(stderr, "Input file has more than 10 lines\n");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < n; i++) {
            matrix[i][n - rowCount] = line[i];
        }
    }

    if (rowCount < 10) {
        fprintf(stderr, "The matrix has fewer than 10 rows\n");
        return EXIT_FAILURE;
    }

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            fprintf(stdout, "%c", matrix[j][k]);
        }
        fprintf(stdout, "\n");
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening input file or file disrupted");
        return EXIT_FAILURE;
    }

    int tmp = rotateMatrix(file);

    if (fclose(file) != 0) {
        perror("Error closing the file");
        return EXIT_FAILURE;
    }

    return tmp;
}

