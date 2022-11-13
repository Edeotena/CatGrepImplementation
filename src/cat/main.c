#include <stdio.h>

typedef struct {
    int eOpt;
    int bOpt;
    int nOpt;
    int sOpt;
    int tOpt;
    int vOpt;
} CatOptions;

void catNoOptions(char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("n/a");
    } else {
        int c = EOF;
        while ((c = getc(file)) != EOF) {
            printf("%c", c);
        }
    }
}

void catWithOptions(char *argv[], CatOptions *options) {

}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("n/a");
    }
    else if (argc == 2) {
        catNoOptions(argv[1]);
    } else {
        CatOptions options  ;
        catWithOptions(argv, &options);
    }
}