CC := gcc
CFLAGS := -Wall -Wextra -O3

test: test.c
	$(CC) -o $@ $^ $(CFLAGS)
