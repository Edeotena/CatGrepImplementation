#include <stdio.h>
#include <string.h>

#define BAD_OPTIONS (-1)
#define BAD_FILES (-2)

typedef struct {
    int eOpt;
    int bOpt;
    int nOpt;
    int sOpt;
    int tOpt;
    int vOpt;
} CatOptions;

CatOptions getOptions(int argc, char *argv[], int *code);

void catWithOptions(int argc, char *argv[], const CatOptions *options, int *code);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("n/a");
    } else {
        int code = 0;
        CatOptions options = getOptions(argc, argv, &code);
        if (code != BAD_OPTIONS) {
            if (options.bOpt == 1) {
                options.nOpt = 0;
            }
            catWithOptions(argc, argv, &options, &code);
        } else {
            printf("n/a");
        }
    }
}

CatOptions getOptions(int argc, char *argv[], int *code) {
    CatOptions result = {};
    for (int i = 1; i < argc && *code == 0; ++i) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            size_t len = strlen(argv[i]);
            for (size_t j = 1; j < len && *code == 0; ++j) {
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
    return result;
}

void catWithOptions(int argc, char *argv[], const CatOptions *options, int *code) {
    int string_number = 1, empty_string = 0;
    for (int i = 1; i < argc && *code == 0; ++i) {
        if (argv[i][0] != '\0') {
            FILE *file = fopen(argv[i], "r");
            if (file != NULL) {
                char ch, last_ch = '\n';
                while ((ch = (char) getc(file)) != EOF) {
                    if (options->sOpt != 1 || ch != '\n' || empty_string != 1) {
                        if ((options->bOpt == 1 && last_ch == '\n' && ch != '\n') ||
                            (options->nOpt == 1 && options->bOpt != 1 && last_ch == '\n')) {
                            printf("%6d\t", string_number);
                            ++string_number;
                        }
                        empty_string = (last_ch == '\n' && ch == '\n') ? 1 : 0;
                        printf("%c", ch);
                    }
                    last_ch = ch;
                }
            } else {
                *code = BAD_FILES;
            }
        }
    }
}
