CC = gcc
FLAGS = -Wall -Wextra -Werror
TARGET = s21_cat
FILES = s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(FLAGS) -D_GNU_SOURCE $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
