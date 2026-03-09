CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

all: test

test: tests/tests.o src/ring_buffer.o
	$(CC) $(CFLAGS) -o test tests/tests.o

tests/tests.o: tests/tests.c include/ring_buffer.h
	$(CC) $(CFLAGS) -c tests/tests.c -o tests/tests.o

src/ring_buffer.o: src/ring_buffer.c include/ring_buffer.h
	$(CC) $(CFLAGS) -c src/ring_buffer.c -o src/ring_buffer.o

clean:
	rm -rf src/*.o tests/*.o test

.PHONY: all clean