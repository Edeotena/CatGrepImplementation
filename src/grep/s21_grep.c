#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_ERROR (-1)
#define FILE_OPEN_ERROR (-2)
#define UNRECOGNIZED_OPTION (-3)

#define buffer 4096

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

// Получение опций и паттернов из argv
GrepOptions getOptions(int argc, char *argv[], int *code);
// Добавление паттерна в структуру
int writePattern(GrepOptions *options, const char *pattern, int ignore_case);
// Чтение паттернов из файла и добавление их в структуру
int writePatternsFile(GrepOptions *options, char *file_name, int ignore_case);
// Функция определяющая работу по опциям
void grepWithOptions(const GrepOptions *options, int argc, char **argv);
// Проверка строки на наличие паттернов
int isPatternIn(const GrepOptions *options, const char *line);
// Начальная проверка на опции -e -f -i
int checkSpecialOptions(int argc, char *argv[], int *iOpt);
// Освобождение выделенной памяти в структуре
void freeRegex(GrepOptions *options);
// Безопасное освобождение строки
void safeFree(char *line);
// Работа команды греп при опции l
size_t handleLOption(const GrepOptions *options, FILE *file);
// Работа команды греп при опции c
size_t handleCOption(const GrepOptions *options, FILE *file);
// Работа команды греп при опции o
void handleOOption(const GrepOptions *options, FILE *file,
                   const char *file_name);
// Обработка символа в опцию
void getSingleOptions(GrepOptions *options, char ch, int *code);
// Обычная работа команды греп с учетом опций
void handleUsuall(const GrepOptions *options, FILE *file,
                  const char *file_name);
// Найти regmatch_t с минимальным rg_so с учетом всех паттернов
int findMinOffset(const GrepOptions *options, const char *line,
                  regmatch_t *min);

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int code = 0;
    GrepOptions options = getOptions(argc, argv, &code);
    if (code == 0) {
      if (options.vOpt == 1) {
        options.oOpt = 0;
      }
      grepWithOptions(&options, argc, argv);
    } else {
      fprintf(stderr, "n/a");
    }
    freeRegex(&options);
  } else {
    fprintf(stderr, "n/a");
  }
}

void freeRegex(GrepOptions *options) {
  for (int i = 0; i < options->patternsCount; ++i) {
    regfree(&options->patterns[i]);
  }
  if (options->patterns != NULL) {
    free(options->patterns);
  }
}

void safeFree(char *line) {
  if (line != NULL) {
    free(line);
  }
}

int writePattern(GrepOptions *options, const char *pattern, int ignore_case) {
  int result = 0;
  if (options->patternsCap == options->patternsCount) {
    options->patternsCap =
        options->patternsCap == 0 ? 3 : options->patternsCap * 2;
    regex_t *new_ptr = (regex_t *)realloc(
        options->patterns, options->patternsCap * sizeof(regex_t));
    if (new_ptr == NULL) {
      result = ALLOC_ERROR;
    } else {
      options->patterns = new_ptr;
    }
  }
  if (result == 0) {
    result =
        regcomp(&options->patterns[options->patternsCount], pattern,
                ignore_case == 0 ? REG_EXTENDED : REG_ICASE | REG_EXTENDED);
    ++options->patternsCount;
  }
  if (result != 0) {
    freeRegex(options);
  }
  return result;
}

int writePatternsFile(GrepOptions *options, char *file_name, int ignore_case) {
  int result = 0;
  FILE *file = fopen(file_name, "r");
  if (file != NULL) {
    char *line = (char *)calloc(buffer, sizeof(char));
    if (line != NULL) {
      while (fgets(line, buffer, file) != NULL && result == 0) {
        if (line[strlen(line) - 1] == '\n') {
          result = line[strlen(line) - 1] = '\0';
        }
        writePattern(options, line, ignore_case);
      }
      safeFree(line);
    }
    fclose(file);
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
  } else if (ch != 'e' && ch != 'f' && ch != 'i') {
    *code = UNRECOGNIZED_OPTION;
  }
}

GrepOptions getOptions(int argc, char *argv[], int *code) {
  int next_pattern = 0, next_pattern_file = 0, files_counter = 0;
  GrepOptions result = {0};
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
    if (i != search_string) {
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
  }
  if (next_pattern == 1 || next_pattern_file == 1) {
    *code = UNRECOGNIZED_OPTION;
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
      for (size_t j = 1; j < len; ++j) {
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
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    if (isPatternIn(options, line) == 1) {
      res = 1;
    }
  }
  safeFree(line);
  return res;
}

size_t handleCOption(const GrepOptions *options, FILE *file) {
  char *line = NULL;
  size_t n, res = 0;
  while (getline(&line, &n, file) != EOF) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    if (isPatternIn(options, line) == 1) {
      ++res;
    }
  }
  safeFree(line);
  return res;
}

int findMinOffset(const GrepOptions *options, const char *line,
                  regmatch_t *min) {
  int found = 0;
  regmatch_t match[2];
  for (int i = 0; i < options->patternsCount; ++i) {
    if (regexec(&options->patterns[i], line, 2, match, 0) == 0) {
      if (found == 0 || match[0].rm_so < min->rm_so) {
        *min = match[0];
        found = 1;
      }
    }
  }
  return found;
}

void handleOOption(const GrepOptions *options, FILE *file,
                   const char *file_name) {
  char *line = NULL;
  size_t n = 0, string_number = 0;
  while (getline(&line, &n, file) != EOF) {
    ++string_number;
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    char *line_with_offset = line;
    regmatch_t founded = {0};
    int first = 1;
    while (findMinOffset(options, line_with_offset, &founded) != 0) {
      if (first == 1) {
        first = 0;
        if (options->hOpt == 0) {
          printf("%s:", file_name);
        }
        if (options->nOpt == 1) {
          printf("%zu:", string_number);
        }
      }
      printf("%.*s\n", (int)(founded.rm_eo - founded.rm_so),
             line_with_offset + founded.rm_so);
      line_with_offset += founded.rm_eo;
    }
  }
  safeFree(line);
}

void handleUsuall(const GrepOptions *options, FILE *file,
                  const char *file_name) {
  char *line = NULL;
  size_t n, string_number = 1;
  while (getline(&line, &n, file) != EOF) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    if (isPatternIn(options, line) == 1) {
      if (options->hOpt == 0) {
        printf("%s:", file_name);
      }
      if (options->nOpt == 1) {
        printf("%zu:", string_number);
      }
      printf("%s\n", line);
    }
    ++string_number;
  }
  safeFree(line);
}

void grepWithOptions(const GrepOptions *options, int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '\0') {
      FILE *file = fopen(argv[i], "r");
      if (file != NULL) {
        size_t count = 0;
        if (options->cOpt == 1) {
          count = handleCOption(options, file);
          if (options->hOpt == 0) {
            printf("%s:", argv[i]);
          }
          printf("%zu\n", options->lOpt == 0 ? count : (count == 0 ? 0 : 1));
        }
        if (options->lOpt == 1) {
          if (count != 0 ||
              (options->cOpt == 0 && handleLOption(options, file) == 1)) {
            printf("%s\n", argv[i]);
          }
        }
        if (options->oOpt == 1 && options->lOpt == 0 && options->cOpt == 0) {
          handleOOption(options, file, argv[i]);
        } else if (options->lOpt == 0 && options->cOpt == 0) {
          handleUsuall(options, file, argv[i]);
        }
        fclose(file);
      } else if (options->sOpt != 1) {
        fprintf(stderr, "grep: %s: No such file\n", argv[i]);
      }
    }
  }
}
