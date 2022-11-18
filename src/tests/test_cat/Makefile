CC = gcc
FLAGS = -Wall -Wextra -Werror --std=c11 -fanalyzer -pedantic
TARGET = s21_cat
FILES = ./../../cat/s21_cat.c

all: clean s21_cat test

s21_cat: $(FILES)
	$(CC) $(FLAGS) -D_GNU_SOURCE $(FILES) -o $(TARGET)

clean:
	rm -f $(TARGET)

test:
	bash test_func_cat.sh
