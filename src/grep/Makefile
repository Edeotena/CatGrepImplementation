CC = gcc
FLAGS = -Wall -Wextra -Werror
TARGET = s21_grep
FILES = s21_grep.c

all: s21_grep

s21_grep:
	$(CC) $(FLAGS) -D_GNU_SOURCE $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
