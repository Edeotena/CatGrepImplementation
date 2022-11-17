#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_ERROR (-1)
#define FILE_OPEN_ERROR (-2)
#define UNRECOGNIZED_OPTION (-3)

typedef struct {
    regex_t *patterns;
    int patternsCount;
    int patternsCap;
    int vOpt;
    int cOpt;
    int lOpt;
    int nOpt;
    int hOpt;
    int sOpt;
    int oOpt;
} GrepOptions;

GrepOptions getOptions(int argc, char* argv[], int *code);

int writePattern(GrepOptions *options, const char* pattern, int ignore_case);

int writePatternsFile(GrepOptions *options, char* file_name, int ignore_case);

void grepWithOptions(const GrepOptions *options, int argc, char **argv);

int isPatternIn(const GrepOptions *options, const char *line);

int checkSpecialOptions(int argc, char *argv[], int *iOpt);

void freeRegex(GrepOptions *options);

size_t handleLOption(const GrepOptions *options, FILE *file);

size_t handleSOption(const GrepOptions *options, FILE *file);

void getSingleOptions(GrepOptions *options, char ch, int *code);

void handleUsuall(const GrepOptions *options, FILE *file, const char *file_name);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int code = 0;
        GrepOptions options = getOptions(argc, argv, &code);
        if (code == 0) {
            grepWithOptions(&options, argc, argv);
            freeRegex(&options);
        } else {
            printf("n/a");
        }
    } else {
        printf("n/a");
    }
}

void freeRegex(GrepOptions *options) {
    for (int i = 0; i < options->patternsCount; ++i) {
        regfree(&options->patterns[i]);
    }
    free(options->patterns);
}

// Добавление паттерна к массиву
int writePattern(GrepOptions *options, const char* pattern, int ignore_case) {
    int result = 0;
    if (options->patternsCap == options->patternsCount) {
        options->patternsCap = options->patternsCap == 0 ? 3 : options->patternsCap * 2;
        regex_t *new_ptr = (regex_t*)realloc(options->patterns, options->patternsCap * sizeof(regex_t));
        if (new_ptr == NULL) {
            result = ALLOC_ERROR;
        } else {
            options->patterns = new_ptr;
        }
    }
    if (result == 0) {
        result = regcomp(&options->patterns[options->patternsCount], pattern, ignore_case == 0 ? REG_EXTENDED : REG_ICASE);
        ++options->patternsCount;
    }
    if (result != 0) {
        freeRegex(options);
    }
    return result;
}

int writePatternsFile(GrepOptions *options, char* file_name, int ignore_case) {
    int result = 0;
    FILE *file = fopen(file_name, "r");
    if (file != NULL) {
        char *line;
        size_t len;
        while (getline(&line, &len, file) != EOF) {
            line[strlen(line) - 1] = '\0';
            writePattern(options, line, ignore_case);
        }
    } else {
        result = FILE_OPEN_ERROR;
    }
    return result;
}

void getSingleOptions(GrepOptions *options, char ch, int *code) {
    if (ch == 'v') {
        options->vOpt = 1;
    } else if (ch == 'c') {
        options->cOpt = 1;
    } else if (ch == 'l') {
        options->lOpt = 1;
    } else if (ch == 'n') {
        options->nOpt = 1;
    } else if (ch == 'h') {
        options->hOpt = 1;
    } else if (ch == 's') {
        options->sOpt = 1;
    } else if (ch == 'o') {
        options->oOpt = 1;
    } else if (ch != 'e' && ch != 'f'){
        *code = UNRECOGNIZED_OPTION;
    }
}

GrepOptions getOptions(int argc, char* argv[], int *code) {
    int next_pattern = 0, next_pattern_file = 0, files_counter = 0;
    GrepOptions result = {};
    int search_string = 0, ignore_case = 0;
    if (checkSpecialOptions(argc, argv, &ignore_case) != 1) {
        search_string = 1;
        while (argv[search_string][0] == '-') {
            ++search_string;
        }
        *code = writePattern(&result, argv[search_string], ignore_case);
        argv[search_string][0] = '\0';
    }
    for (int i = 1; i < argc && *code == 0; ++i) {
        if (i == search_string) {
            continue;
        }
        if (argv[i][0] == '-' && next_pattern_file == 0 && next_pattern == 0) {
            size_t len = strlen(argv[i]);
            for (size_t j = 1; j < len; ++j) {
                getSingleOptions(&result, argv[i][j], code);
            }
            next_pattern = argv[i][len - 1] == 'e' ? 1 : 0;
            next_pattern_file = argv[i][len - 1] == 'f' ? 1 : 0;
            argv[i][0] = '\0';
        } else if (next_pattern == 1) {
            *code = writePattern(&result, argv[i], ignore_case);
            next_pattern = 0;
            argv[i][0] = '\0';
        } else if (next_pattern_file == 1) {
            *code = writePatternsFile(&result, argv[i], ignore_case);
            next_pattern_file = 0;
            argv[i][0] = '\0';
        } else {
            ++files_counter;
        }
    }
    if (files_counter == 1) {
        result.hOpt = 1;
    }
    return result;
}

int isPatternIn(const GrepOptions *options, const char *line) {
    int result = 0;
    for (int i = 0; i < options->patternsCount; ++i) {
        if (regexec(&options->patterns[i], line, 0, NULL, 0) == 0) {
            result = 1;
        }
    }
    if (options->vOpt == 1) {
        result = result == 1 ? 0 : 1;
    }
    return result;
}

int checkSpecialOptions(int argc, char *argv[], int *iOpt) {
    int result = 0;
    for (int i = 1; i < argc && result == 0; ++i) {
        if (argv[i][0] == '-') {
            size_t len = strlen(argv[i]);
            for (size_t j = 1; j < len && result == 0; ++j) {
                if (argv[i][j] == 'i') {
                    *iOpt = 1;
                } else if (argv[i][j] == 'e' || argv[i][j] == 'f') {
                    result = 1;
                }
            }
        }
    }
    return result;
}

size_t handleLOption(const GrepOptions *options, FILE *file) {
    char *line = NULL;
    size_t n, res = 0;
    while (getline(&line, &n, file) != EOF && res == 0) {
        line[strlen(line) - 1] = '\0';
        if (isPatternIn(options, line) == 1) {
            res = 1;
        }
    }
    return res;
}

size_t handleSOption(const GrepOptions *options, FILE *file) {
    char *line = NULL;
    size_t n, res = 0;
    while (getline(&line, &n, file) != EOF) {
        line[strlen(line) - 1] = '\0';
        if (isPatternIn(options, line) == 1) {
            ++res;
        }
    }
    return res;
}

void handleUsuall(const GrepOptions *options, FILE *file, const char *file_name) {
    char *line = NULL;
    size_t n, string_number = 1;
    while (getline(&line, &n, file) != EOF) {
        line[strlen(line) - 1] = '\0';
        if (isPatternIn(options, line) == 1) {
            if (options->hOpt == 0) {
                printf("%s:", file_name);
            }
            if (options->nOpt == 1) {
                printf("%zu:", string_number);
                ++string_number;
            }
            printf("%s\n", line);
        }
    }
}

void grepWithOptions(const GrepOptions *options, int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '\0') {
            FILE *file = fopen(argv[i], "r");
            if (file != NULL) {
                if (options->lOpt == 1) {
                    if (handleLOption(options, file) == 1) {
                        printf("%s\n", argv[i]);
                    }
                } else if (options->cOpt == 1) {
                    size_t count = handleSOption(options, file);
                    if (options->hOpt == 0) {
                        printf("%s:", argv[i]);
                    }
                    printf("%zu\n", count);
                } else {
                    handleUsuall(options, file, argv[i]);
                }
                fclose(file);
            }
        }
    }
}
