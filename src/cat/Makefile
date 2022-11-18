CC = gcc
FLAGS = -Wall -Wextra -Werror
TARGET = s21_cat
FILES = main.c

all: s21_cat

s21_cat:
	$(CC) $(FLAGS) -D_GNU_SOURCE $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
