CC = clang
CFLAGS = -Wall -g

all: clean rgbcp

rgbcp: rx tx

rx:
	$(CC) -o rx src/rx.c
tx:
	$(CC) -o tx -lncurses src/tx.c

.PHONY: clean
clean:
	rm -f rx
	rm -rf rx.dSYM
	rm -f tx
	rm -rf tx.dSYM
