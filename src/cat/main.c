#include <stdio.h>
#include <string.h>

#define BAD_OPTIONS (-1)

typedef struct {
    int eOpt;
    int bOpt;
    int nOpt;
    int sOpt;
    int tOpt;
    int vOpt;
} CatOptions;

CatOptions getOptions(int argc, char *argv[], int *code);

void catWithOptions(char *argv[], const CatOptions *options);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("n/a");
    } else {
        int code = 0;
        CatOptions options = getOptions(argc, argv, &code);
        if (code != BAD_OPTIONS) {
            catWithOptions(argv, &options);
        } else {
            printf("n/a");
        }
    }
}

CatOptions getOptions(int argc, char *argv[], int *code) {
    CatOptions result;
    for (int i = 1; i < argc && *code != BAD_OPTIONS; ++i) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            size_t len = strlen(argv[i]);
            for (size_t j = 1; j < len && *code != BAD_OPTIONS; ++j) {
                if (argv[i][j] == 'b') {
                    result.bOpt = 1;
                } else if (argv[i][j] == 'e') {
                    result.eOpt = 1;
                    result.vOpt = 1;
                } else if (argv[i][j] == 'E') {
                    result.eOpt = 1;
                } else if (argv[i][j] == 'n') {
                    result.nOpt = 1;
                } else if (argv[i][j] == 's') {
                    result.sOpt = 1;
                } else if (argv[i][j] == 't') {
                    result.tOpt = 1;
                    result.vOpt = 1;
                } else if (argv[i][j] == 'T') {
                    result.tOpt = 1;
                } else {
                    *code = BAD_OPTIONS;
                }
            }
            argv[i][0] = '\0';
        } else if (argv[i][1] == '-') {
            if (strcmp(argv[i], "--number-nonblank") == 0) {
                result.bOpt = 1;
            } else if (strcmp(argv[i], "--number") == 0) {
                result.nOpt = 1;
            } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
                result.sOpt = 1;
            } else {
                *code = BAD_OPTIONS;
            }
            argv[i][0] = '\0';
        }
    }
}

void catWithOptions(char *argv[], const CatOptions *options) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("n/a");
    } else {
        int c = EOF, last_char ;
        while ((c = getc(file)) != EOF) {
            printf("%c", c);
        }
    }
}
