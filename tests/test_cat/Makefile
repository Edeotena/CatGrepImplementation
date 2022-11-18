all: clean build test

clean:
	rm s21_cat

build:
	gcc -Wall -Wextra -Werror ../../src/cat/main.c -o s21_cat

test:
	sh test_func_cat.sh
