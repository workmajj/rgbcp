NAME = wink

CC = clang
CFLAGS = -Wall -g

all: clean wink

wink:
	$(CC) -o $(NAME) $(NAME).c

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
