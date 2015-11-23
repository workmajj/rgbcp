CC = clang
CFLAGS = -MD -Wall -Werror -std=c99

.PHONY: all
all: tx rx

tx: src/tx.o src/types.h
	$(CC) $(CFLAGS) src/tx.o -lncurses -o tx

rx: src/rx.o src/types.h
	$(CC) $(CFLAGS) src/rx.o -o rx

.PHONY: clean
clean:
	rm -f tx rx
	rm -f src/*.o src/*.d
