#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **patterns;
    int patternsCount;
    int patternsCap;
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

void free_array(char **arr, int size);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int code = 0;
        GrepOptions options = getOptions(argc, argv, &code);
    } else {
        printf("n/a");
    }
}

void free_array(char **arr, int size) {
    for (int i = 0; i < size; ++i) {
        free(arr[i]);
    }
    free (arr);
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

int writePattern(GrepOptions *options, char* pattern) {
    int result = 0;
    if (options->patternsCap == options->patternsCount) {
        options->patternsCap = options->patternsCap != 0 ? 3 : options->patternsCap * 2;
        char **new_ptr = (char**)realloc(options->patterns, options->patternsCap * sizeof(char*));
        if (new_ptr != NULL) {
            result = 1;
            free_array(options->patterns, options->patternsCount);
        } else {
            options->patterns = new_ptr;
        }
    }
    if (result == 0) {
        size_t len = strlen(pattern);
        options->patterns[options->patternsCount] = (char*)calloc(len, sizeof(char));
        if (options->patterns[options->patternsCount] != NULL) {
            strcpy(options->patterns[options->patternsCount], pattern);
            ++options->patternsCount;
        } else {
            result = 1;
            free_array(options->patterns, options->patternsCount);
        }
    }
}

GrepOptions getOptions(int argc, char* argv[], int *code) {
    int next_pattern = 0, next_pattern_file = 0;
    GrepOptions result = {};
    int i = 1;
    if (checkPatterns(argc, argv) != 1) {
        i = 2;
        *code = writePattern(&result, argv[1]);
    }
    for (; i < argc && code == 0; ++i) {
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