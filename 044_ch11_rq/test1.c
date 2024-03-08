#include <stdio.h>

int main() {
    FILE *fp = fopen("file.txt", "r");
    if (fp == NULL) {
        printf("无法打开文件\n");
        return 1;
    }

    char c;
    while ((c = fgetc(f)) != EOF) { //
        printf("%c", c);

    fclose(fp);
    return 0;
}


