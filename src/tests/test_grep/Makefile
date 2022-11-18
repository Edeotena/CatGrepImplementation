CC = gcc
FLAGS = -Wall -Wextra -Werror --std=c11 -fanalyzer -pedantic
TARGET = s21_grep
FILES = ../../grep/s21_grep.c

all: clean s21_grep test

s21_grep: $(FILES)
	$(CC) $(FLAGS) -D_GNU_SOURCE $(FILES) -o $(TARGET)

clean:
	rm -f $(TARGET)

test: $(TARGER)
	bash test_func_grep.sh
