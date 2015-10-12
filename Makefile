CC = clang
CFLAGS = -Wall -g

all: clean wink

wink: rx tx

rx:
	$(CC) -o rx rx.c
tx:
	$(CC) -o tx tx.c

.PHONY: clean
clean:
	rm -f rx
	rm -rf rx.dSYM
	rm -f tx
	rm -rf tx.dSYM
