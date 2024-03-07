.PHONY: run debug clean

CC=cc
CFLAGS=--std=c11 -g -Wall -Werror -O3
LDFLAGS=

main: main.c
	$(CC) -o $@ $(CFLAGS) $? $(LDFLAGS)

run: main
	./main

debug: main
	gdb ./main

clean:
	rm -f ./main

