#include <stdio.h>

typedef struct {
    int eOpt;
    int bOpt;
    int nOpt;
    int sOpt;
    int tOpt;
    int vOpt;
} CatOptions;

void catNoOptions(const char *filename);

CatOptions getOptions(int argc, char *argv[], int *filename_index);

void catWithOptions(const char *filename, const CatOptions *options);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("n/a");
    } else if (argc == 2) {
        catNoOptions(argv[1]);
    } else {
        int filename_index = 0;
        CatOptions options = getOptions(argc, argv, &filename_index);
        catWithOptions(argv[filename_index], &options);
    }
}

void catNoOptions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("n/a");
    } else {
        int c = EOF;
        while ((c = getc(file)) != EOF) {
            printf("%c", c);
        }
    }
}