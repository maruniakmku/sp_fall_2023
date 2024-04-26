.PHONY: run debug clean

CC=cc
CFLAGS=-Wall -g -Og
LDFLAGS=

main: main.c
	$(CC) -o $@ $(CFLAGS) $? $(LDFLAGS)

run: main
	./main

debug: main
	gdb ./main

clean:
	rm -f ./main

