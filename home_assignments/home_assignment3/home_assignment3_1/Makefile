.PHONY: run debug clean test

CC=cc
CFLAGS=-Wall -g -O0

main: main.c
	$(CC) -o $@ $(CFLAGS) $?

run: main
	./main

debug: main
	gdb ./main

clean:
	rm -f ./main

