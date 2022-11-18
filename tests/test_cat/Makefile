all: clean build test

clean:
	rm s21_cat

build:
	gcc -Wall -Wextra -Werror ../../src/cat/s21_cat.c -o s21_cat

test:
	sh test_func_cat.sh
