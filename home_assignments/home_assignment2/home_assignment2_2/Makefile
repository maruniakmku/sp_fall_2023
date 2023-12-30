.PHONY: run debug clean test

CC=cc
CFLAGS=-Wall -Werror -g -Og

main: main.c
	$(CC) -o $@ $(CFLAGS) $?

run: main
	./main

debug: main
	gdb ./main

clean:
	rm -f ./main

test: main
	./test.sh

