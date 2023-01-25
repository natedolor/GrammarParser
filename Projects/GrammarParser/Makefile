CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -Ofast

.PHONY: all clean
all: banhammer

banhammer: banhammer.o city.o ht.o ll.o node.o bf.o bv.o parser.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

spotless:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.[ch]
