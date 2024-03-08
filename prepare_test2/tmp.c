#include <stdio.h>

int main() {
    FILE * f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    int c;
    int tmp;
    while ((c = fgetc(f)) != EOF) {
        printf("打印%c", (char)c);
	tmp = c;
	printf("再打印%d",tmp);
    }
    fclose(f);
    return 0;
}

