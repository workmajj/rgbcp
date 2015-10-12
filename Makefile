CC = clang
CFLAGS = -Wall -g

all: clean rx

rx:
	$(CC) -o rx rx.c

.PHONY: clean
clean:
	rm -f rx
	rm -rf rx.dSYM
