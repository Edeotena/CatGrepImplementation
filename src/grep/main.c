#include <stdio.h>
#include <regex.h>
#include <string.h>

typedef struct {
    char **patterns;
    int patternsCount;
    int iOpt;
    int vOpt;
    int cOpt;
    int lOpt;
    int nOpt;
    int hOpt;
    int sOpt;
    int oOpt;
} GrepOptions;

GrepOptions getOptions(int argc, char* argv[], int *code);

int checkPatterns(int argc, char *argv[]);

int writePattern(GrepOptions *options, char* pattern);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int code = 0;
        GrepOptions options = getOptions(argc, argv, &code);
    } else {
        printf("n/a");
    }
}

// Проверка на то, есть ли флаги -e / -f
int checkPatterns(int argc, char *argv[]) {
    int result = 0;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            size_t len = strlen(argv[i]);
            for (int j = 1; j < len; ++j) {
                if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    result = 1;
                }
            }
        }
    }
    return result;
}

GrepOptions getOptions(int argc, char* argv[], int *code) {
    int next_pattern = 0, next_pattern_file = 0;
    GrepOptions result = {};
    int i = 1;
    if (checkPatterns(argc, argv) != 1) {
        i = 2;
        writePattern(&result, argv[1]);
    }
    for (; i < argc; ++i) {
        if (argv[i][0] == '-' && next_pattern_file == 0 && next_pattern == 0) {
            next_pattern = next_pattern_file = 0;
            size_t len = strlen(argv[i]);
            for (int j = 1; j < len; ++j) {
                if (argv[i][j] == 'i') {
                    result.iOpt = 1;
                } else if (argv[i][j] == 'v') {
                    result.vOpt = 1;
                } else if (argv[i][j] == 'c') {
                    result.cOpt = 1;
                } else if (argv[i][j] == 'l') {
                    result.lOpt = 1;
                } else if (argv[i][j] == 'n') {
                    result.nOpt = 1;
                } else if (argv[i][j] == 'h') {
                    result.hOpt = 1;
                } else if (argv[i][j] == 's') {
                    result.sOpt = 1;
                } else if (argv[i][j] == 'o') {
                    result.oOpt = 1;
                }
            }
            if (argv[i][len - 1] == 'e') {
                next_pattern = 1;
            } else if (argv[i][len - 1] == 'f') {
                next_pattern_file = 1;
            }
        } else {

        }

    }
}