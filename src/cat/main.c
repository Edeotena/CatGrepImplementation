#include <stdio.h>

void catNoOptions(char *argv[]) {
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("n/a");
    } else {
        int c = EOF;
        while ((c = getc(file)) != EOF) {
            printf("%c", c);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        catNoOptions(argv);
    } else {
        printf("n/a");
    }
}